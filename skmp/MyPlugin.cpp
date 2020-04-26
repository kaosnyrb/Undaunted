#include "MyPlugin.h"
#include "skse64/PapyrusDefaultObjectManager.h"
#include <skse64\PapyrusCell.h>
#include <stdlib.h>
#include <skse64\PapyrusGame.h>
#include <skse64\PapyrusObjectReference.cpp>
#include <skse64\PapyrusObjects.h>
#include <skse64\PapyrusQuest.h>

namespace Undaunted {
	VMClassRegistry* _registry;
	TESWorldSpace* Interiors;

	TESObjectREFR* SpawnLocref = NULL;
	TESWorldSpace* worldspace = NULL;
	TESObjectREFR* xmarkerref = NULL;

	TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID)
	{
		TESObjectCELL* parentCell = (*g_thePlayer)->parentCell;
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("GetObjectInCurrentCell Num Ref: %i", numberofRefs);
		for (int i = 0; i < numberofRefs; i++)
		{
			TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);
			if (ref != NULL)
			{
				if (ref->formID != NULL)
				{
					if (ref->formID == formID)
					{
						_MESSAGE("ref->formID == formID");
						return ref;
					}
				}
			}
		}
		return NULL;
	}

	
	//Use Sparingly.
	TESObjectREFR* GetRefObjectFromWorld(UInt32 formID)
	{
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
									for (int i = 0; i < numberofRefs; i++)
									{
										TESObjectREFR* ref = papyrusCell::GetNthRef(cell, i, 0);
										if (ref != NULL)
										{
											if (ref->formID != NULL)
											{
												if (ref->formID == formID)
												{
													return ref;
												}
											}
										}
									}
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
		return NULL;
	}

	TESObjectREFR* GetRandomObjectInCell(TESObjectCELL* cell)
	{
		int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
		_MESSAGE("GetRandomObjectInCell Num Ref: %i", numberofRefs);
		if (numberofRefs == 0)return NULL;
		while(true)
		{
			TESObjectREFR* ref = papyrusCell::GetNthRef(cell, rand() % numberofRefs, 0);
			if (ref != NULL)
			{
				return ref;
			}
		}
		return NULL;
	}

	void SpawnMonsters(int count, UInt32 Type)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
			return;

		TESObjectCELL* parentCell = (*g_thePlayer)->parentCell;
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("Num Ref: %i", numberofRefs);
		while (count > 0)
		{
			for (int i = 0; i < numberofRefs; i++)
			{
				TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);

				if (ref != NULL)
				{
					if (rand() % 10 + 1 > 5)
					{
						// Spawn
						TESObjectREFR* spawned = PlaceAtMe_Native(_registry, 1, ref, spawnForm, 1, false, false);
						count--;
						if (spawned == NULL)
							return;
					}
				}
				if (count <= 0)
				{
					return;
				}
			}
		}
	}
	
	void SpawnMonstersInCell(int count, UInt32 Type, TESObjectCELL* parentCell)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid");
			return;
		}

		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		//No leveledChar entries. Skip
		if (numberofRefs == 0)
		{
			return;
		}
		_MESSAGE("SpawnMonstersInCell Num Ref: %i", numberofRefs);
		while (count > 0)
		{
			for (int i = 0; i < numberofRefs; i++)
			{
				TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);
				
				if (ref != NULL)
				{
					if (rand() % 10 + 1 > 5)
					{
						// Spawn
						TESObjectREFR* spawned = PlaceAtMe_Native(_registry, 1, ref, spawnForm, 1, false, false);
						_MESSAGE("Spawned At: %i , %i, %i", spawned->pos.x, spawned->pos.y, spawned->pos.z);
						count--;
						if (spawned == NULL)
							return;
					}
				}
				if (count <= 0)
				{
					return;
				}
			}
		}
	}

	void SpawnMonstersAtTarget(int count, UInt32 Type, TESObjectREFR* Target)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
			return;

		_MESSAGE("spawntype");
		for (int i = 0; i < count; i++)
		{
			TESObjectREFR* spawned = PlaceAtMe_Native(_registry, 1, Target, spawnForm, 1, false, false);
		}
		_MESSAGE("Target");
	}

	void MoveMarkerToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
	{
		_MESSAGE("Moving %08X to %08X in %08X", object->formID, cell->formID, worldspace->formID);
		UInt32 nullHandle = *g_invalidRefHandle;
		NiPoint3 finalPos(pos);
		//finalPos += object->pos;

		MoveRefrToPosition(object, &nullHandle, cell, worldspace, &finalPos, &rot);

	}

	float MyTest(StaticFunctionTag *base) {

		TESForm* spawnForm = LookupFormByID(0x00039CFC);
		if (spawnForm == NULL)
			return 1;

		//0x00014132 Avirso
		//0x01001DFA Fred
		//0x06001DFC XMarker
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
							//SpawnMonstersInCell(1, 0x06001DFA, cell);
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
		SpawnMonstersAtTarget(5, 0x00039CFC, xmarkerref);
		//&test->worldSpace;

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
		TESQuest* quest = papyrusQuest::GetQuest(base, "1_Undaunted_TestQuest01");
//		quest->aliases[0]->
		return 2;
	}
	
	bool RegisterFuncs(VMClassRegistry* registry) {
		_registry = registry;
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, float>("MyTest", "MyPluginScript", Undaunted::MyTest, registry));

		return true;
	}
} 
