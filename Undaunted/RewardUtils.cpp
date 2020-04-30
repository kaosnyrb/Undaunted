#include "RewardUtils.h"
#include <time.h>

UInt32 Undaunted::GetReward()
{
	srand(time(NULL));
	DataHandler* dataHandler = DataHandler::GetSingleton();
	const ModInfo* modInfo = dataHandler->LookupModByName("Skyrim.esm");

	int type = rand() % 2;
	TESObjectWEAP* weapon = NULL;
	TESObjectARMO* armour = NULL;
	switch (type)
	{
		case 0:
			dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
			return weapon->formID;
		case 1:
			dataHandler->armors.GetNthItem(rand() % dataHandler->armors.count, armour);
			return armour->formID;
		default:
			dataHandler->weapons.GetNthItem(rand() % dataHandler->weapons.count, weapon);
			return weapon->formID;
	}
}
