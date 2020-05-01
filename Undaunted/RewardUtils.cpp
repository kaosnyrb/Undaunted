#include "RewardUtils.h"
#include <time.h>
#include <set>

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
			ModInfo* mod;
			dataHandler->modList.loadedMods.GetNthItem(rand() % dataHandler->modList.loadedMods.count, mod);
			_MESSAGE("Loading Reward from: %s ", mod->name);
			const ModInfo* modInfo = dataHandler->LookupModByName(mod->name);
			int type = rand() % 2;
			TESObjectWEAP* weapon = NULL;
			TESObjectARMO* armour = NULL;
			//type = 0;
			switch (type)
			{
			case 0:
				if (dataHandler->weapons.count == 0)
				{
					_MESSAGE("No weapons found in: %s ", mod->name);
					break;
				}
				dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
				if (!weapon->IsPlayable()) continue;
				if (!weapon->Has3D()) continue;
				if (weapon->value.value == 0) continue;
				if (weapon->templateForm) continue;
				if (!IsWeaponLevelOk(weapon, playerlevel)) continue;
				return weapon->formID;
			case 1:
				if (dataHandler->armors.count == 0)
				{
					_MESSAGE("No armors found in: %s ", mod->name);
					break;
				}		
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
				if (weapon->templateForm) continue;
				return weapon->formID;
			}
		}
	}

	bool IsWeaponLevelOk(TESObjectWEAP* weapon, UInt32 playerlevel)
	{
		UInt16 attackDamage = weapon->damage.attackDamage * 100;
		UInt32 Moneyvalue = weapon->value.value;
		int targetMaxLevel = 40;
		float levelcoeffient = playerlevel;
		float minDamage = 400;
		float maxDamage = 2800;
		float partcoeffient = (maxDamage - minDamage) / targetMaxLevel;
		_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);

		int minValueForPart = 1;
		int maxValueForPart = 2563;
		float valuecoeffient = (maxValueForPart - minValueForPart) / targetMaxLevel;
		//_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);
		for (int i = 0; i < weapon->keyword.numKeywords; i++)
		{
			if (_stricmp(weapon->keyword.keywords[i]->keyword.data, "DaedricArtifact") == 0)return false;
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
		int targetMaxLevel = 40;
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
				minArmourForPart = 2000;//Hide
				maxArmourForPart = 4100;//Dragonscale 
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minArmourForPart = 500;
				maxArmourForPart = 1200;
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minArmourForPart = 500;
				maxArmourForPart = 1200;
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minArmourForPart = 1000;
				maxArmourForPart = 1700;
			}
			if ((mask & armour->bipedObject.kPart_Shield) != 0)
			{
				minArmourForPart = 1500;
				maxArmourForPart = 2900;
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
				minArmourForPart = 2500;//Iron
				maxArmourForPart = 4900;//Daedric 
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minArmourForPart = 1000;
				maxArmourForPart = 1800;
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minArmourForPart = 1000;
				maxArmourForPart = 1800;
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minArmourForPart = 1500;
				maxArmourForPart = 2300;
			}
			if ((mask & armour->bipedObject.kPart_Shield) != 0)
			{
				minArmourForPart = 2000;
				maxArmourForPart = 3600;
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
		if (weightClass == 00000002)
		{
			if ((mask & armour->bipedObject.kPart_Body) != 0)
			{
				minValueForPart = 1;
				maxValueForPart = 2563;
			}
			if ((mask & armour->bipedObject.kPart_Feet) != 0)
			{
				minValueForPart = 1;
				maxValueForPart = 250; 
			}
			if ((mask & armour->bipedObject.kPart_Hands) != 0)
			{
				minValueForPart = 1;
				maxValueForPart = 170;
			}
			if ((mask & armour->bipedObject.kPart_Head) != 0)
			{
				minValueForPart = 50;
				maxValueForPart = 505;
			}
			if ((mask & armour->bipedObject.kPart_Ring) != 0)
			{
				minValueForPart = 5;
				maxValueForPart = 3500;
			}
			if ((mask & armour->bipedObject.kPart_Circlet) != 0)
			{
				minValueForPart = 50;
				maxValueForPart = 500;
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

