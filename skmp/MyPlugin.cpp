#include "MyPlugin.h"
#include "LocationUtils.h"
#include "SpawnUtils.h"

namespace Undaunted {
	VMClassRegistry* _registry;
	TESWorldSpace* Interiors;

//	TESObjectREFR* SpawnLocref = NULL;
	TESWorldSpace* worldspace = NULL;
	TESObjectREFR* xmarkerref = NULL;
	tList<TESObjectREFR> bountyenemies;
	bool DEBUGMODE = true;

	float MyTest(StaticFunctionTag *base) {
		if (xmarkerref == NULL)
		{
			xmarkerref = GetRefObjectFromWorld(0x06001DFE);
			_MESSAGE("Got Marker %08X", xmarkerref->formID);
		}
		DataHandler* handler = DataHandler::GetSingleton();
		_MESSAGE("RegionList Count: %08X", handler->regionList->Count());

		UInt32 regioncount = handler->regionList->Count();
		for (UInt32 i = 0; i < regioncount; i++)
		{
			//Some regions are dodgy
			if (i != 0x00000033 && i != 0x00000036 && i != 0x0000009B && i != 0x00000110)
			{
				_MESSAGE("processing worldSpace %08X", i);
				TESRegion* test = (TESRegion*)handler->regionList->GetNthItem(i);
				if (test != NULL)
				{
					if (test->worldSpace == NULL)
					{
						_MESSAGE("worldSpace %08X is null", i);
					}
					else
					{
						TESObjectCELL* cell = test->worldSpace->unk088;
						if (cell != NULL)
						{
							_MESSAGE("Regioncell form id %08X", cell->formID);
							if (cell->formID == 0x00000D74)
							{
								int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
								if (numberofRefs > 0)
								{
									worldspace = test->worldSpace;
									TESObjectREFR* target = GetRandomObjectInCell(cell);
									MoveMarkerToWorldCell(xmarkerref, cell, worldspace, target->pos, NiPoint3(0, 0, 0));
								}
							}
						}
						else
						{
							_MESSAGE("unk088 is null", i);
						}
					}

				}
				else
				{
					_MESSAGE("RegionList %08X is null", i);
				}
			}
		}
		_MESSAGE("Finished Regions");
		bountyenemies = SpawnMonstersAtTarget(_registry, 5, 0x00039CFC, xmarkerref);
		_MESSAGE("Enemy Count : %08X ", bountyenemies.Count());
		//Interiors
		/*
		if (SpawnLocref == NULL)
		{
			TESObjectCELL* here = (*g_thePlayer)->parentCell;
			_MESSAGE("Here form id %08X", here->formID);
			_MESSAGE("Cell list Size %08X", DataHandler::GetSingleton()->cellList.m_size);
			UInt32 cellcount = DataHandler::GetSingleton()->cellList.m_size;
			for (int i = 0; i < cellcount; i++)
			{
				TESObjectCELL* parentCell = DataHandler::GetSingleton()->cellList.m_data[i];
				_MESSAGE("Cell form id %08X", parentCell->formID);
				_MESSAGE("Cell form id %08X", parentCell->formID);
				int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
				_MESSAGE("Num Ref: %i", numberofRefs);
				SpawnMonstersInCell(1, 0x06001DFC, parentCell);
			}
		}
		*/
//		TESQuest* quest = papyrusQuest::GetQuest(base, "1_Undaunted_TestQuest01");
		return 2;
	}

	bool isBountyComplete(StaticFunctionTag* base) {
		if (bountyenemies.Count() == 0)
			return false;

		bool alldead = true;
		for (UInt32 i = 0; i < bountyenemies.Count(); i++)
		{
			if (!bountyenemies.GetNthItem(i)->IsDead(1))
			{
				return false;
			}
		}
		
		return true;
	}

	
	bool RegisterFuncs(VMClassRegistry* registry) {
		_registry = registry;
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, float>("MyTest", "MyPluginScript", Undaunted::MyTest, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, bool>("isBountyComplete", "bountyCompleteScript", Undaunted::isBountyComplete, registry));

		return true;
	}
} 
