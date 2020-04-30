#include "RewardUtils.h"
#include <time.h>

UInt32 Undaunted::GetReward()
{
	srand(time(NULL));
	bool found = false;
	while (!found)
	{
		DataHandler* dataHandler = DataHandler::GetSingleton();
		ModInfo* mod;
		dataHandler->modList.loadedMods.GetNthItem(rand() % dataHandler->modList.loadedMods.count, mod);
		_MESSAGE("Loading Reward from: %s ", mod->name);
		const ModInfo* modInfo = dataHandler->LookupModByName(mod->name);

		int type = rand() % 2;
		TESObjectWEAP* weapon = NULL;
		TESObjectARMO* armour = NULL;
		switch (type)
		{
		case 0:
			if (dataHandler->weapons.count == 0)
			{
				_MESSAGE("No weapons found in: %s ", mod->name);
				break;
			}
			dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
			return weapon->formID;
		case 1:
			if (dataHandler->armors.count == 0)
			{
				_MESSAGE("No armors found in: %s ", mod->name);
				break;
			}
			dataHandler->armors.GetNthItem(rand() % dataHandler->armors.count, armour);
			return armour->formID;
		default:
			dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
			return weapon->formID;
		}
	}

}
