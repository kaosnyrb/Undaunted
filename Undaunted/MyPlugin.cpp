#include "MyPlugin.h"
#include <Undaunted\StartupManager.h>
#include <algorithm>
#include <string>
#include "UnStringList.h"

namespace Undaunted {

	UInt32 hook_CreateBounty(StaticFunctionTag* base) {
		int result = BountyManager::getInstance()->activebounties.length;
		_MESSAGE("hook_CreateBounty result: %08X", result);
		Bounty newBounty = Bounty();
		BountyManager::getInstance()->activebounties.AddItem(newBounty);
		return result;
	}

	// Triggers a new bounty stage to start.
	float hook_StartBounty(StaticFunctionTag* base, UInt32 BountyId, bool nearby) {
		_MESSAGE("hook_StartBounty BountyId: %08X", BountyId);
		BountyManager::getInstance()->StartBounty(BountyId,nearby, "",NULL,"");
		return 2;
	}

	// Triggers a new Elite bounty stage to start.
	float hook_StartEliteBounty(StaticFunctionTag* base, UInt32 BountyId, bool nearby) {
		_MESSAGE("hook_StartEliteBounty BountyId: %08X", BountyId);
		BountyManager::getInstance()->StartBounty(BountyId, nearby, "", NULL, "", "ELITE");
		return 2;
	}

	// Triggers a new bounty stage to start with a certain name.
	float hook_StartNamedBounty(StaticFunctionTag* base, UInt32 BountyId, bool nearby, BSFixedString bountyName) {
		_MESSAGE("hook_StartNamedBounty BountyId: %08X", BountyId);
		BountyManager::getInstance()->StartBounty(BountyId,nearby, bountyName.Get(),NULL,"");
		return 2;
	}


	// Triggers a new bounty stage to start with a certain name.
	float hook_restartNamedBounty(StaticFunctionTag* base, UInt32 BountyId,BSFixedString bountyName) {
		_MESSAGE("hook_restartNamedBounty BountyId: %08X", BountyId);
		BountyManager::getInstance()->restartBounty(BountyId,bountyName.Get());
		return 2;
	}

	
	float hook_StartNamedBountyNearRef(StaticFunctionTag* base, UInt32 BountyId,bool nearby, BSFixedString bountyName, TESObjectREFR* ref, BSFixedString WorldSpaceName) {
		_MESSAGE("hook_StartNamedBountyNearRef BountyId: %08X", BountyId);
		BountyManager::getInstance()->StartBounty(BountyId,nearby, bountyName.Get(),ref,WorldSpaceName);
		return 2;
	}


	// Fill out the WorldList, this checks the loaded world cells and finds the persistant reference cells.
	// This takes a while so we only do this once at the start
	bool hook_InitSystem(StaticFunctionTag* base, UInt32 playerLevel)
	{
		DataHandler* dataHandler = GetDataHandler();
		_MESSAGE("Mod Count: %08X", dataHandler->modList.loadedMods.count);
		for (int i = 0; i < dataHandler->modList.loadedMods.count; i++)
		{
			ModInfo* mod;
			dataHandler->modList.loadedMods.GetNthItem(i, mod);
			_MESSAGE("Listing Mods: %s ", mod->name);
		}
		LoadSettings();
		LoadGroups();
		LoadRifts();
		BuildWorldList();
		SetPlayerLevel(playerLevel);
		BountyManager::getInstance()->isReady = 2;
		_MESSAGE("ReadyState: %i ", BountyManager::getInstance()->isReady);
		return true;
	}

	// A check to see if the Init call has finished
	UInt32 hook_isSystemReady(StaticFunctionTag* base)
	{
		return BountyManager::getInstance()->isReady;
	}

	bool hook_ClaimStartupLock(StaticFunctionTag* base)
	{
		_MESSAGE("ReadyState: %i ", BountyManager::getInstance()->isReady);
		if (BountyManager::getInstance()->isReady == 0)
		{
			BountyManager::getInstance()->isReady = 1;
			return true;
		}
		return false;
	}

	// Check if all the bounty objectives have been complete
	bool hook_isBountyComplete(StaticFunctionTag* base, UInt32 BountyId) {
		_MESSAGE("Starting Bounty Check %08X ", BountyId);
		if (BountyManager::getInstance()->activebounties.length < BountyId)
		{
			return false;
		}
		return BountyManager::getInstance()->BountyUpdate(BountyId);
	}
	// Tell the bounty system that this object should be marked as complete
	void hook_SetGroupMemberComplete(StaticFunctionTag* base, TESObjectREFR* taget)
	{
		_MESSAGE("hook_SetGroupMemberComplete");
		//This actually works as it's using the object reference. The object should only be in one bounty.
		for (int i = 0; i < BountyManager::getInstance()->activebounties.length; i++)
		{
			BountyManager::getInstance()->activebounties.data[i].bountygrouplist.SetGroupMemberComplete(taget->formID);
		}	
	}

	BSFixedString hook_getBountyName(StaticFunctionTag* base, UInt32 BountyId) {
		_MESSAGE("Starting Bounty Check");
		if (BountyManager::getInstance()->activebounties.length > BountyId)
		{
			return BountyManager::getInstance()->activebounties.data[BountyId].bountygrouplist.questText.c_str();
		}
		return "NO BOUNTIES";
	}	

	// Pass the reference to the XMarker that we use as the quest target and the target of the placeatme calls
	bool hook_SetXMarker(StaticFunctionTag* base, UInt32 BountyId, TESObjectREFR* marker) {
		_MESSAGE("hook_SetXMarker %08X ", BountyId);
		BountyManager::getInstance()->activebounties.data[BountyId].xmarkerref = marker;
		return true;
	}

	// Pass the reference to the quest objective message. This allows us to edit it from the code.
	bool hook_SetBountyMessageRef(StaticFunctionTag* base, UInt32 BountyId, BGSMessage* ref) {
		_MESSAGE("hook_SetBountyMessageRef %08X ", BountyId);
		BountyManager::getInstance()->activebounties.data[BountyId].bountymessageref = ref;
		return true;
	}

	// For reasons as yet unknown, some of the regions in memory cause crash to desktops. We have to skip processing these. Hoping to fix this.
	bool hook_AddBadRegion(StaticFunctionTag* base, UInt32 region) {
		AddBadRegionToConfig(region);
		return true;
	}

	// Process the Group header line. We return the groups position which we can use to add to later.
	UInt32 hook_AddGroup(StaticFunctionTag* base, BSFixedString questText, BSFixedString modRequirement, UInt32 minLevel, UInt32 maxLevel, UInt32 playerLevel){
		//Mod required is not loaded
		bool foundmod = false;
		DataHandler* dataHandler = GetDataHandler();
		for (int i = 0; i < dataHandler->modList.loadedMods.count; i++)
		{
			ModInfo* mod;
			dataHandler->modList.loadedMods.GetNthItem(i, mod);
			if (strcmp(mod->name, modRequirement.Get()) == 0)
			{
				foundmod = true;
				break;
			}
		}
		if (!foundmod)
		{
			_MESSAGE("%s: Mod %s is not loaded", questText.Get(), modRequirement.Get());
			return -1;
		}
		return AddGroup(questText.Get(),minLevel,maxLevel, UnStringlist());
	}

	// Add a member to a group.
	void hook_AddMembertoGroup(StaticFunctionTag* base, UInt32 groupid, UInt32 member, BSFixedString BountyType, BSFixedString ModelFilepath) {
		GroupMember newMember = GroupMember();
		newMember.FormId = member;
		newMember.BountyType = BountyType;
		newMember.ModelFilepath = ModelFilepath;
		AddMembertoGroup(groupid, newMember);
	}

	// Given a mod name and a FormId - load order, return the actualy form id
	UInt32 hook_GetModForm(StaticFunctionTag* base, BSFixedString ModName, UInt32 FormId){
		DataHandler* dataHandler = GetDataHandler();
		const ModInfo* modInfo = dataHandler->LookupModByName(ModName.c_str());
		if (modInfo != NULL)
		{
			FormId = (modInfo->modIndex << 24) + FormId;
			if (modInfo->IsFormInMod(FormId))
			{
				return FormId;
			}
			else
			{
				_MESSAGE("FormId  %08X Not Found in %s", FormId, ModName.Get());
				return UInt32();
			}
		}
		_MESSAGE("Mod Not Found: %s", ModName.Get());
		return UInt32();
	}

	// Return a reward form. We seed the random data with the offset + time so that we can spawn multiple things at once.
	TESForm* hook_SpawnRandomReward(StaticFunctionTag* base, UInt32 rewardOffset, UInt32 playerlevel)
	{
		_MESSAGE("hook_SpawnRandomReward");
		UInt32 rewardid = GetReward(rewardOffset, playerlevel);
		_MESSAGE("RewardID: %08X", rewardid);
		TESForm* spawnForm = LookupFormByID(rewardid);
		return spawnForm;
//		PlaceAtMe_Native(BountyManager::getInstance()->_registry, 1, target, spawnForm, 1, false, false);
	}



	// Pass in a config value
	void hook_SetConfigValue(StaticFunctionTag* base, BSFixedString key, BSFixedString value)
	{
		AddConfigValue(key.Get(), value.Get());
	}
	// Returns an int that is in the config
	UInt32 hook_GetConfigValueInt(StaticFunctionTag* base, BSFixedString key)
	{
		return GetConfigValueInt(key.Get());
	}
	
	BSFixedString hook_GetPlayerWorldSpaceName(StaticFunctionTag* base)
	{
		_MESSAGE("hook_GetPlayerWorldSpaceName");
		return GetCurrentWorldspaceName().Get();
	}


	bool hook_isPlayerInWorldSpace(StaticFunctionTag* base, BSFixedString worldspacename)
	{
		_MESSAGE("hook_isPlayerInWorldSpace");
		return _stricmp(GetCurrentWorldspaceName().Get(), worldspacename.Get()) == 0;
	}

	// Currently unused, checks if the object reference is in the current bounty.
	bool hook_IsGroupMemberUsed(StaticFunctionTag* base, TESObjectREFR* target)
	{
		_MESSAGE("hook_IsGroupMemberUsed");
		/*
		//Is this reference in the current bounty? If it isn't we can get rid of it.
		for (int i = 0; i < BountyManager::getInstance()->bountygrouplist.length; i++)
		{
			if (BountyManager::getInstance()->bountygrouplist.data[i].objectRef->formID == target->formID)
			{
				return true;
			}
		}*/
		return false;
	}

	// The player has fast travelled. This causes cells which are marked to reset to reset.
	// This means we can take all bounties off the blacklist.
	void hook_PlayerTraveled(StaticFunctionTag* base, float distance)
	{
		_MESSAGE("hook_PlayerTraveled %f hours", distance);
		//If a bounty is running and we fast travel then clean it up.
		//If it hasn't started yet we don't need to worry.
//		if (BountyManager::getInstance()->activebounties.data[BountyId].bountywave > 0)
	//	{
			//BountyManager::getInstance()->ClearBountyData(BountyId);
		//}
		if (distance > 1.5f)
		{
			BountyManager::getInstance()->ResetBountiesRan();
		}
	}

	// Triggered when leaving a microdungeon. Tells all the doors that the microdungeon has been completed.
	void hook_SetScriptedDoorsComplete(StaticFunctionTag* base)
	{
		_MESSAGE("Starting hook_SetBountyComplete");
		for (int i = 0; i < BountyManager::getInstance()->activebounties.length; i++)
		{
			for (int j = 0; j < BountyManager::getInstance()->activebounties.data[i].bountygrouplist.length; j++)
			{
				const char* type = BountyManager::getInstance()->activebounties.data[i].bountygrouplist.data[j].BountyType.c_str();
				if (strcmp(type, "SCRIPTEDDOOR") == 0)
				{
					BountyManager::getInstance()->activebounties.data[i].bountygrouplist.data[j].isComplete = true;
				}
			}
		}
	}

	// Returns the references of all the spawned objects of a certain type
	VMResultArray<TESObjectREFR*> hook_GetBountyObjectRefs(StaticFunctionTag* base, UInt32 BountyId,BSFixedString bountyType)
	{
		_MESSAGE("hook_GetBountyObjectRefs %08X ", BountyId);
		std::string type = bountyType.c_str();
		std::transform(type.begin(), type.end(), type.begin(), ::toupper);

		VMResultArray<TESObjectREFR*> resultsarray = VMResultArray<TESObjectREFR*>();

		if (BountyManager::getInstance()->activebounties.length < BountyId)
		{
			return resultsarray;
		}

		if (strcmp("DELETE", type.c_str()) == 0)
		{
			_MESSAGE("hook_GetBountyObjectRefs DELETE");
			for (int i = 0; i < BountyManager::getInstance()->deleteList.length; i++)
			{
				resultsarray.push_back(BountyManager::getInstance()->deleteList.data[i].objectRef);
			}
			BountyManager::getInstance()->ClearDeleteList();
			return resultsarray;
		}

		for (int i = 0; i < BountyManager::getInstance()->activebounties.data[BountyId].bountygrouplist.length; i++)
		{
			if (strcmp(BountyManager::getInstance()->activebounties.data[BountyId].bountygrouplist.data[i].BountyType.c_str(), type.c_str()) == 0 ||
				strcmp(bountyType.Get(),"ALL") == 0)
			{
				if (BountyManager::getInstance()->activebounties.data[BountyId].bountygrouplist.data[i].objectRef != NULL)
				{
					resultsarray.push_back(BountyManager::getInstance()->activebounties.data[BountyId].bountygrouplist.data[i].objectRef);
				}
			}
		}
		_MESSAGE("hook_GetBountyObjectRefs %08X Success", BountyId);
		return resultsarray;
	}



	BSFixedString hook_GetRandomBountyName(StaticFunctionTag* base)
	{
		_MESSAGE("hook_GetRandomBountyName");
		GroupList list = GetRandomGroup();
		BSFixedString result = BSFixedString();
		result = list.questText.c_str();
		return result;
	}

	void hook_CaptureCellData(StaticFunctionTag* base)
	{
		_MESSAGE("hook_CaptureCellData");
		CaptureArea();		
	}

	TESObjectREFR* hook_SpawnRift(StaticFunctionTag* base, UInt32 BountyId, TESObjectREFR* Startpoint)
	{
		_MESSAGE("hook_SpawnRift");
		return BountyManager::getInstance()->StartRift(BountyId, Startpoint);
	}

	

	bool RegisterFuncs(VMClassRegistry* registry) {

		BountyManager::getInstance()->_registry = registry;
		//General
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, UInt32>("CreateBounty", "Undaunted_SystemScript", Undaunted::hook_CreateBounty, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, float, UInt32,bool>("StartBounty", "Undaunted_SystemScript", Undaunted::hook_StartBounty, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, float, UInt32, bool>("StartEliteBounty", "Undaunted_SystemScript", Undaunted::hook_StartEliteBounty, registry));		

		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, float, UInt32, bool, BSFixedString>("StartNamedBounty", "Undaunted_SystemScript", Undaunted::hook_StartNamedBounty, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, float, UInt32, BSFixedString>("RestartNamedBounty", "Undaunted_SystemScript", Undaunted::hook_restartNamedBounty, registry));


		registry->RegisterFunction(
			new NativeFunction5 <StaticFunctionTag, float, UInt32,bool, BSFixedString, TESObjectREFR*, BSFixedString>("StartNamedBountyNearRef", "Undaunted_SystemScript", Undaunted::hook_StartNamedBountyNearRef, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32>("GetBountyName", "Undaunted_SystemScript", Undaunted::hook_getBountyName, registry));


		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("isBountyComplete", "Undaunted_SystemScript", Undaunted::hook_isBountyComplete, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, UInt32, TESObjectREFR*>("SetXMarker", "Undaunted_SystemScript", Undaunted::hook_SetXMarker, registry));
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, bool, UInt32,BGSMessage*>("SetBountyMessageRef", "Undaunted_SystemScript", Undaunted::hook_SetBountyMessageRef, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, UInt32>("isSystemReady", "Undaunted_SystemScript", Undaunted::hook_isSystemReady, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, bool>("ClaimStartupLock", "Undaunted_SystemScript", Undaunted::hook_ClaimStartupLock, registry));

	

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("InitSystem", "Undaunted_SystemScript", Undaunted::hook_InitSystem, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, BSFixedString>("GetRandomBountyName", "Undaunted_SystemScript", Undaunted::hook_GetRandomBountyName, registry));


		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void,float>("PlayerTraveled", "Undaunted_SystemScript", Undaunted::hook_PlayerTraveled, registry));


		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, BSFixedString>("GetPlayerWorldSpaceName", "Undaunted_SystemScript", Undaunted::hook_GetPlayerWorldSpaceName, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("isPlayerInWorldSpace", "Undaunted_SystemScript", Undaunted::hook_isPlayerInWorldSpace, registry));

		//Config
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BSFixedString, BSFixedString>("SetConfigValue", "Undaunted_SystemScript", Undaunted::hook_SetConfigValue, registry));
		
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetConfigValueInt", "Undaunted_SystemScript", Undaunted::hook_GetConfigValueInt, registry));

		//Regions
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("AddBadRegion", "Undaunted_SystemScript", Undaunted::hook_AddBadRegion, registry));

		//Groups
		registry->RegisterFunction(
			new NativeFunction5 <StaticFunctionTag, UInt32, BSFixedString, BSFixedString, UInt32, UInt32, UInt32>("AddGroup", "Undaunted_SystemScript", Undaunted::hook_AddGroup, registry));

		registry->RegisterFunction(
			new NativeFunction4 <StaticFunctionTag, void,UInt32,UInt32, BSFixedString, BSFixedString>("AddMembertoGroup", "Undaunted_SystemScript", Undaunted::hook_AddMembertoGroup, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, UInt32, BSFixedString, UInt32>("GetModForm", "Undaunted_SystemScript", Undaunted::hook_GetModForm, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, TESObjectREFR*>("SetGroupMemberComplete", "Undaunted_SystemScript", Undaunted::hook_SetGroupMemberComplete, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>("IsGroupMemberUsed", "Undaunted_SystemScript", Undaunted::hook_IsGroupMemberUsed, registry));

		registry->RegisterFunction(
			new NativeFunction2<StaticFunctionTag, VMResultArray<TESObjectREFR*>, UInt32, BSFixedString>("GetBountyObjectRefs", "Undaunted_SystemScript", Undaunted::hook_GetBountyObjectRefs, registry));

		registry->RegisterFunction(
			new NativeFunction0<StaticFunctionTag, void>("SetScriptedDoorsComplete", "Undaunted_SystemScript", Undaunted::hook_SetScriptedDoorsComplete, registry));

		//Rewards
		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, TESForm*, UInt32, UInt32>("SpawnRandomReward", "Undaunted_SystemScript", Undaunted::hook_SpawnRandomReward, registry));


		//Rifts
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, void>("CaptureCellData", "Undaunted_SystemScript", Undaunted::hook_CaptureCellData, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, TESObjectREFR*, UInt32, TESObjectREFR*>("SpawnRift", "Undaunted_SystemScript", Undaunted::hook_SpawnRift, registry));


		return true;
	}
}
