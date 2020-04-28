#include "MyPlugin.h"
#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"

namespace Undaunted {
	VMClassRegistry* _registry;
	//TESWorldSpace* Interiors;

	TESObjectREFR* xmarkerref = NULL;
	GroupList bountygrouplist;
	WorldCell bountyworldcell;

	int bountywave = 0;

	float StartBounty(StaticFunctionTag* base, BSFixedString WorldspaceName) {
		if (xmarkerref == NULL)
		{
			_MESSAGE("NO XMARKER SET");
			return 0;
		}
		//Cleanup previous bounties
		bountywave = 0;
		bountygrouplist = GroupList();

		BuildWorldList();
		bountyworldcell = GetNamedWorldCell(WorldspaceName);
		_MESSAGE("target is set. Moving marker: WorldSpace: %s Cell: %08X ", bountyworldcell.world->editorId.Get(), bountyworldcell.cell->formID);
		TESObjectREFR* target = GetRandomObjectInCell(bountyworldcell.cell);
		MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, target->pos, NiPoint3(0, 0, 0));
		//Splitting setting the bounty location and the spawning.
		/*
		IntList group = GetRandomGroup();
		for (int i = 0; i < group.length; i++)
		{
			_MESSAGE("Groupid : %08X ", group.data[i]);
		}
		bountyenemies = SpawnMonstersAtTarget(_registry, group, xmarkerref);
		_MESSAGE("Enemy Count : %08X ", bountyenemies.Count());
		*/
		return 2;
	}

	bool hook_isBountyComplete(StaticFunctionTag* base) {
		if (bountywave == 0 && bountyworldcell.world != NULL)
		{
			//Is the player in the right worldspace?
			if (_stricmp((*g_thePlayer)->currentWorldSpace->editorId.Get(), bountyworldcell.world->editorId.Get()) == 0)
			{
				_MESSAGE("Player in Worldspace");
				//Check the distance to the XMarker
				NiPoint3 distance = (*g_thePlayer)->pos - xmarkerref->pos;
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				_MESSAGE("Distance to marker: %f", distvector.Magnitude());
				if (distvector.Magnitude() < 5000)
				{
					GroupList group = GetRandomGroup();
					for (int i = 0; i < group.length; i++)
					{
						_MESSAGE("Groupid : %08X ", group.data[i]);
					}
					bountygrouplist = SpawnGroupAtTarget(_registry, group, xmarkerref, bountyworldcell.cell, bountyworldcell.world);
					_MESSAGE("Enemy Count : %08X ", bountygrouplist.length);
					bountywave = 1;
				}
			}
		}

		if (bountygrouplist.length == 0)
			return false;

		bool alldead = true;
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "Enemy") == 0)
			{
				if (!bountygrouplist.data[i].objectRef->IsDead(1))
				{
					MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, bountygrouplist.data[i].objectRef->pos, NiPoint3(0, 0, 0));
					return false;
				}
			}
		}
		
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "BountyDecoration") == 0)
			{
				MoveRefToWorldCell(bountygrouplist.data[i].objectRef, (*g_thePlayer)->parentCell, (*g_thePlayer)->currentWorldSpace,
					NiPoint3(bountygrouplist.data[i].objectRef->pos.x, bountygrouplist.data[i].objectRef->pos.y, -10000), NiPoint3(0, 0, 0));
				//bountygrouplist.data[i].objectRef->handleRefObject.Release();
			}
		}
		return true;
	}
	bool hook_SetXMarker(StaticFunctionTag* base, TESObjectREFR* marker) {
		xmarkerref = marker;
		return true;
	}

	bool hook_AddBadRegion(StaticFunctionTag* base, UInt32 region) {
		AddBadRegionToConfig(region);
		return true;
	}

	UInt32 hook_AddGroup(StaticFunctionTag* base){
		return AddGroup();
	}

	void hook_AddMembertoGroup(StaticFunctionTag* base, UInt32 groupid, UInt32 member, BSFixedString BountyType) {
		GroupMember newMember = GroupMember();
		newMember.FormId = member;
		newMember.BountyType = BountyType;
		AddMembertoGroup(groupid, newMember);
	}

	UInt32 hook_GetModForm(StaticFunctionTag* base, BSFixedString ModName, UInt32 FormId){
		DataHandler* dataHandler = DataHandler::GetSingleton();
		_MESSAGE("Mod Count: %08X", dataHandler->modList.loadedMods.count);
		for (int i = 0; i < dataHandler->modList.loadedMods.count; i++)
		{
			ModInfo* mod;
			dataHandler->modList.loadedMods.GetNthItem(i,mod);
			_MESSAGE("Listing Mods: %s ", mod->name);
		}
		const ModInfo* modInfo = dataHandler->LookupModByName(ModName.c_str());
		if (modInfo != NULL)
		{
			_MESSAGE("Mod Found: %s ", modInfo->name);
			_MESSAGE("Mod Index: %08X ", modInfo->modIndex);
			FormId = (modInfo->modIndex << 24) + FormId;
			_MESSAGE("Computed FormId: %08X ", FormId);

			//<< 1
			if (modInfo->IsFormInMod(FormId))
			{
				return FormId;
			}
			else
			{
				_MESSAGE("FormId Not Found");
				return UInt32();
			}
		}
		_MESSAGE("Mod Not Found");
		return UInt32();
	}

	bool RegisterFuncs(VMClassRegistry* registry) {
		_registry = registry;
		//General
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, float, BSFixedString>("StartBounty", "Undaunted_StartBountyScript", Undaunted::StartBounty, registry));
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, bool>("isBountyComplete", "Undaunted_bountyCompleteScript", Undaunted::hook_isBountyComplete, registry));
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>("SetXMarker", "Undaunted_SetXMarkerScript", Undaunted::hook_SetXMarker, registry));

		//Regions
		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("AddBadRegion", "Undaunted_AddBadRegionScript", Undaunted::hook_AddBadRegion, registry));

		//Groups
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, UInt32>("AddGroup", "Undaunted_AddGroupScript", Undaunted::hook_AddGroup, registry));

		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, void,UInt32,UInt32, BSFixedString>("AddMembertoGroup", "Undaunted_AddMembertoGroupScript", Undaunted::hook_AddMembertoGroup, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, UInt32, BSFixedString, UInt32>("GetModForm", "Undaunted_GetModFormScript", Undaunted::hook_GetModForm, registry));

		return true;
	}
}
