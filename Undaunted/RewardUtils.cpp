#include "RewardUtils.h"
#include <Undaunted\ConfigUtils.h>

namespace Undaunted
{
	int GetRewardType() {
		int choice_weight[] = 
		{ 
			GetConfigValueInt("RewardWeaponWeight"),
			GetConfigValueInt("RewardArmourWeight"),
			GetConfigValueInt("RewardPotionWeight"),
			GetConfigValueInt("RewardScrollWeight"),
			GetConfigValueInt("RewardIngredientWeight"),
			GetConfigValueInt("RewardBookWeight"),
			GetConfigValueInt("RewardMiscWeight")
		};
		int numberofchoices = 7;
		int sum_of_weight = 0;
		for (int i = 0; i < numberofchoices; i++) {
			sum_of_weight += choice_weight[i];
		}
		int rnd = rand() % sum_of_weight;
		for (int i = 0; i < numberofchoices; i++) {
			if (rnd < choice_weight[i])
				return i;
			rnd -= choice_weight[i];
		}
	}

	int loopcount = 0;
	bool isFormInBlacklist(UInt32 formid)
	{
		auto blacklist = getRewardBlacklist();
		DataHandler* dataHandler = GetDataHandler();
		for (int i = 0; i < blacklist.length; i++)
		{
			auto mod = dataHandler->LookupModByName(blacklist.data[i].value.c_str());
			if (mod != NULL)
			{
				if (mod->IsFormInMod(formid))
				{
					//loopcount++;
					//srand(time(NULL) + loopcount);
					return true;
				}
			}
		}
		return false;
	}

	UInt32 LastReward = 0;

	UInt32 GetReward(UInt32 rewardOffset, UInt32 playerlevel)
	{
		srand(time(NULL) + rewardOffset);
		DataHandler* dataHandler = GetDataHandler();
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
			int type = 0;
			bool foundvalidrewardtype = false;
			while (!foundvalidrewardtype)
			{
				type = GetRewardType();
				if (type == 6 && GetConfigValueInt("RewardAllowMiscItems") == 0)
				{
					continue;
				}
				foundvalidrewardtype = true;
			}
			TESObjectWEAP* weapon = NULL;
			TESObjectARMO* armour = NULL;
			AlchemyItem* potion = NULL;
			ScrollItem* scroll = NULL;
			IngredientItem* ingre = NULL;
			TESObjectBOOK* book = NULL;
			TESObjectMISC* misc = NULL;
			//type = 3;
			switch (type)
			{
			case 0:
				dataHandler->armors.GetNthItem(rand() % dataHandler->armors.count, armour);
				if (exclude.find(armour) != exclude.end())
				if (!armour->IsPlayable()) continue;
				if (armour->templateArmor) continue;
				if (armour->value.value <= 10) continue;
				if (!IsArmourLevelOk(armour, playerlevel))continue;
				if (armour->formID == LastReward) continue;
				if (isFormInBlacklist(armour->formID)) continue;
				LastReward = armour->formID;
				return armour->formID;
			case 1:
				dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
				if (!weapon->IsPlayable()) continue;
				if (!weapon->Has3D()) continue;
				if (weapon->value.value == 0) continue;
				if (weapon->templateForm) continue;
				if (!IsWeaponLevelOk(weapon, playerlevel)) continue;
				if (weapon->formID == LastReward) continue;
				if (isFormInBlacklist(weapon->formID)) continue;
				LastReward = weapon->formID;
				return weapon->formID;
			case 2:
				dataHandler->potions.GetNthItem(rand() % dataHandler->potions.count, potion);
				if (potion->formID == LastReward) continue;
				if (isFormInBlacklist(potion->formID)) continue;
				LastReward = potion->formID;
				return potion->formID;
			case 3:
				dataHandler->scrolls.GetNthItem(rand() % dataHandler->scrolls.count, scroll);
				if (!scroll->IsPlayable()) continue;
				if (!scroll->Has3D()) continue;
				if (scroll->value.value == 0) continue;
				if (scroll->formID == LastReward) continue;
				if (isFormInBlacklist(scroll->formID)) continue;
				LastReward = scroll->formID;
				return scroll->formID;
			case 4:
				dataHandler->ingredients.GetNthItem(rand() % dataHandler->ingredients.count, ingre);
				if (!ingre->IsPlayable()) continue;
				if (!ingre->Has3D()) continue;
				if (ingre->value.value == 0) continue;
				if (ingre->formID == LastReward) continue;
				if (isFormInBlacklist(ingre->formID)) continue;
				LastReward = ingre->formID;
				return ingre->formID;
			case 5:
				dataHandler->books.GetNthItem(rand() % dataHandler->books.count, book);
				if (!book->IsPlayable()) continue;
				if (!book->Has3D()) continue;
				if (book->value.value == 0) continue;
				if (book->value.value <= 50) continue;
				if (book->value.value >= 2000) continue;
				if (book->formID == LastReward) continue;
				if (isFormInBlacklist(book->formID)) continue;
				LastReward = book->formID;
				return book->formID;
			case 6:
				dataHandler->miscObjects.GetNthItem(rand() % dataHandler->miscObjects.count, misc);
				if (!misc->IsPlayable()) continue;
				if (!misc->Has3D()) continue;
				if (misc->value.value == 0) continue;
				if (misc->value.value <= 50) continue;
				if (misc->formID == LastReward) continue;
				if (isFormInBlacklist(misc->formID)) continue;
				LastReward = misc->formID;
				return misc->formID;
			default:
				dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
				if (!weapon->IsPlayable()) continue;
				if (!weapon->Has3D()) continue;
				if (weapon->value.value == 0) continue;
				if (weapon->templateForm) continue;
				if (!IsWeaponLevelOk(weapon, playerlevel)) continue;
				if (weapon->formID == LastReward) continue;
				if (isFormInBlacklist(weapon->formID)) continue;
				LastReward = weapon->formID;
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
		//_MESSAGE("levelcoeffient: %f , partcoeffient: %f", levelcoeffient, partcoeffient);

		int minValueForPart = GetConfigValueInt("RewardWeaponMinValue");
		int maxValueForPart = GetConfigValueInt("RewardWeaponMaxValue");
		float valuecoeffient = (maxValueForPart - minValueForPart) / targetMaxLevel;
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
		for (int i = 0; i < armour->keyword.numKeywords; i++)
		{
			if (_stricmp(armour->keyword.keywords[i]->keyword.data, "DaedricArtifact") == 0 && GetConfigValueInt("RewardAllowDaedricArtifacts") == 1)return false;
			if (_stricmp(armour->keyword.keywords[i]->keyword.data, "ArmorShield") == 0 && GetConfigValueInt("RewardAllowShields") == 0)return false;
			if (_stricmp(armour->keyword.keywords[i]->keyword.data, "Dummy") == 0) return false;
		}
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
				if (GetConfigValueInt("RewardAllowShields") == 0) return false;
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
				if (GetConfigValueInt("RewardAllowShields") == 0) return false;
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

