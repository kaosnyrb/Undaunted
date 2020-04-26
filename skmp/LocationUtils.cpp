#include "LocationUtils.h"

namespace Undaunted {
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

	int LastRand = 0;
	TESObjectREFR* GetRandomObjectInCell(TESObjectCELL* cell)
	{
		int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
		_MESSAGE("GetRandomObjectInCell Num Ref: %i", numberofRefs);
		if (numberofRefs == 0)return NULL;
		while (true)
		{
			//Random seems to stuck sometimes. Trying to prevent that.
			int Nth = rand() % numberofRefs;
			if (Nth == LastRand) Nth = (rand() % numberofRefs) - 1;
			TESObjectREFR* ref = papyrusCell::GetNthRef(cell, Nth, 0);
			LastRand = Nth;
			if (ref != NULL)
			{
				return ref;
			}
		}
		return NULL;
	}

	void MoveMarkerToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
	{
		_MESSAGE("Moving %08X to %08X in %08X", object->formID, cell->formID, worldspace->formID);
		UInt32 nullHandle = *g_invalidRefHandle;
		NiPoint3 finalPos(pos);
		//finalPos += object->pos;

		MoveRefrToPosition(object, &nullHandle, cell, worldspace, &finalPos, &rot);
	}

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

}