#include "RewardUtils.h"
#include <time.h>
#include <set>
#include <Undaunted\ConfigUtils.h>

namespace Undaunted
{
	UInt32 GetReward(UInt32 playerlevel)
	{
		srand(time(NULL));
		DataHandler* dataHandler = DataHandler::GetSingleton();
		std::set<TESObjectARMO*> exclude;
		TESRace* race = NULL;
		for (UInt32 i = 0; i < dataHandler->races.count; i++)
		{
			dataHandler->races.GetNthItem(i, race);
			if (race->skin.skin)
				exclude.insert(race->skin.skin);
		}
		TESNPC* npc = NULL;
		for (UInt32 i = 0; i < dataHandler->npcs.count; i++)
		{
			dataHandler->npcs.GetNthItem(i, npc);
			if (npc->skinForm.skin)
				exclude.insert(npc->skinForm.skin);
		}
		bool found = false;
		while (!found)
		{
			int type = rand() % 2;
			TESObjectWEAP* weapon = NULL;
			TESObjectARMO* armour = NULL;
			//type = 0;
			switch (type)
			{
			case 0:
				dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
				if (!weapon->IsPlayable()) continue;
				if (!weapon->Has3D()) continue;
				if (weapon->value.value == 0) continue;
				if (weapon->templateForm) continue;
				if (!IsWeaponLevelOk(weapon, playerlevel)) continue;
				return weapon->formID;
			case 1:
				dataHandler->armors.GetNthItem(rand() % dataHandler->armors.count, armour);
				if(exclude.find(armour) != exclude.end())
				if (armour->IsPlayable()) continue;
				if (armour->templateArmor) continue;
				if (armour->value.value == 0) continue;
				if (!IsArmourLevelOk(armour,playerlevel))continue;
				return armour->formID;
			default:
				dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
				if (!weapon->IsPlayable()) continue;
				if (!weapon->Has3D()) continue;
				if (weapon->value.value == 0) continue;
				if (weapon->templateForm) continue;
				if (!IsWeaponLevelOk(weapon, playerlevel)) continue;
				return weapon->formID;
			}
		}
	}

	bool IsWeaponLevelOk(TESObjectWEAP* weapon, UInt32 playerlevel)
	{
		UInt16 attackDamage = weapon->damage.attackDamage * 100;
		UInt32 Moneyvalue = weapon->value.value;
		int targetMaxLevel = GetConfigValueInt("RewardTargetMaxLevel");
		float levelcoeffient = playerlevel + GetConfigValueInt("RewardPlayerLevelBoost");
		float minDamage = GetConfigValueInt("RewardWeaponMinDamage");
		float maxDamage = GetConfigValueInt("RewardWeaponMaxDamage");
		float partcoeffient = (maxDamage - minDamage) / targetMaxLevel;
		_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);

		int minValueForPart = GetConfigValueInt("RewardWeaponMinValue");
		int maxValueForPart = GetConfigValueInt("RewardWeaponMaxValue");
		float valuecoeffient = (maxValueForPart - minValueForPart) / targetMaxLevel;
		//_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);
		for (int i = 0; i < weapon->keyword.numKeywords; i++)
		{			
			if (_stricmp(weapon->keyword.keywords[i]->keyword.data, "DaedricArtifact") == 0 && GetConfigValueInt("RewardAllowDaedricArtifacts") == 1)return false;
			if (_stricmp(weapon->keyword.keywords[i]->keyword.data, "WeapTypeStaff") == 0)return false;//Staffs have low attack damage, but we don't want to spawn them to early.
		}
			
		if (weapon->enchantable.enchantment)
		{
			if (weapon->enchantable.enchantment->data.amount)
			{
				Moneyvalue += weapon->enchantable.enchantment->data.amount;
			}
		}
		if (attackDamage < minDamage + (levelcoeffient * partcoeffient) && Moneyvalue < minValueForPart + (levelcoeffient * partcoeffient))
		{
			return true;
		}
		return false;
	}

	bool IsArmourLevelOk(TESObjectARMO* armour, UInt32 playerlevel)
	{
		UInt32 Armourvalue = armour->armorValTimes100;
		UInt32 Moneyvalue = armour->value.value;
		UInt32 mask = armour->bipedObject.data.parts;
		UInt32 weightClass = armour->bipedObject.data.weightClass;
		int targetMaxLevel = GetConfigValueInt("RewardTargetMaxLevel");
		float levelcoeffient = playerlevel;
//		_MESSAGE("Level %08X, weightClass: %08X, Value: %i, mask: %08X,  Moneyvalue: %08X", playerlevel, weightClass, Armourvalue, mask, Moneyvalue);
		int minArmourForPart = 0;
		int maxArmourForPart = 0;
		//To calculate if an armour is in our level range we figure out a per level amour value for each slot then compare to the item.
		//So if the item is better than what we should have at our level we don't use it.
		//Light Armour
		if (weightClass == 00000000)
		{
			if ((mask & armour->bipedObject.kPart_Body) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Light_Chest_Value_Min");//Hide
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Light_Chest_Value_Max");;//Dragonscale 
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Light_Boot_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Light_Boot_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Light_Hand_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Light_Hand_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Light_Head_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Light_Head_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Shield) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Light_Shield_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Light_Shield_Value_Max");
			}
			float partcoeffient = (maxArmourForPart - minArmourForPart) / targetMaxLevel;
//			_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);
			if (Armourvalue < minArmourForPart + (levelcoeffient * partcoeffient))
			{
				return true;
			}
		}
		//Heavy Armour
		if (weightClass == 00000001)
		{
			if ((mask & armour->bipedObject.kPart_Body) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Chest_Value_Min");//Iron
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Chest_Value_Max");//Daedric 
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Boot_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Boot_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Hand_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Hand_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Head_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Head_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Shield) != 0)
			{
				minArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Shield_Value_Min");
				maxArmourForPart = GetConfigValueInt("Reward_Armour_Heavy_Shield_Value_Max");
			}
			float partcoeffient = (maxArmourForPart - minArmourForPart) / targetMaxLevel;
//			_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);
			if (Armourvalue < minArmourForPart + (levelcoeffient * partcoeffient))
			{
				return true;
			}
		}
		//Clothes
		int minValueForPart = 1;
		int maxValueForPart = 2563;
		if (weightClass == 00000002 && GetConfigValueInt("RewardAllowClothes") == 1 )
		{
			if ((mask & armour->bipedObject.kPart_Body) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Chest_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Chest_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Boot_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Boot_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Hand_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Hand_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Head_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Head_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Ring) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Ring_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Ring_Value_Max");
			}
			if ((mask & armour->bipedObject.kPart_Circlet) != 0)
			{
				minValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Circlet_Value_Min");
				maxValueForPart = GetConfigValueInt("Reward_Armour_Clothes_Circlet_Value_Max");
			}
			float partcoeffient = (maxValueForPart - minValueForPart) / targetMaxLevel;
//			_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);
			if (Moneyvalue < minValueForPart + (levelcoeffient * partcoeffient))
			{
				return true;
			}
		}
		return false;
	}
}

