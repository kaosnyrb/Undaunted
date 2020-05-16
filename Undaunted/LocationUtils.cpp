#include "LocationUtils.h"
#include <Undaunted\ConfigUtils.h>
#include "WorldCellList.h"

namespace Undaunted {
	WorldCellList worldCellList;
	bool worldCellListBuilt = false;



	TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID)
	{
		TESObjectCELL* parentCell = GetPlayer()->parentCell;
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
	/*
	TESObjectREFR* GetRefObjectFromWorld(UInt32 formID)
	{
		DataHandler* handler = GetDataHandler();
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
	}*/

	TESObjectREFR* GetRandomObjectInCell(TESObjectCELL* cell)
	{
		int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
		//_MESSAGE("GetRandomObjectInCell Num Ref: %i", numberofRefs);		
		if (numberofRefs == 0)return NULL;
		while (true)
		{
			int Nth = rand() % numberofRefs;
			TESObjectREFR* ref = papyrusCell::GetNthRef(cell, Nth, 0);
			if (ref != NULL)
			{
				return ref;
			}
		}
		return NULL;
	}

	void BuildWorldList()
	{
		if (worldCellListBuilt)
		{
			return;
		}
		DataHandler* handler = GetDataHandler();
		_MESSAGE("RegionList Count: %08X", handler->regionList->Count());

		IntList badregions = GetBadRegions();

		UInt32 regioncount = handler->regionList->Count();
		for (UInt32 i = 0; i < regioncount; i++)
		{
			//Check for badregion
			bool badRegion = false;
			for (UInt32 j = 0; j < badregions.length; j++)
			{
				if (badregions.data[j] == i)
				{
					badRegion = true;
				}
			}
			//Some regions are dodgy
			if (!badRegion)
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
						_MESSAGE("worldSpace %08X is not null", i);
						_MESSAGE("processing worldSpace %p", test->worldSpace);
						//Yeah. So some regions are really low in the memory stack?
						//They blow up if you try and use them so we filter out worldspace pointers below 000000000002433E
						if ((uintptr_t)test->worldSpace <= 148286)
						{
							_MESSAGE("Low Level Region, Ignoring.");
						}
						else
						{
							TESObjectCELL* cell = test->worldSpace->unk088;
							if (cell != NULL)
							{
								_MESSAGE("unk088 is not null for worldspace %08x", i);
								int numberofRefs = papyrusCell::GetNumRefs(cell, 0);
								if (numberofRefs > 0)
								{
									WorldCell wcell = WorldCell();
									wcell.cell = cell;
									wcell.world = test->worldSpace;

									//Check if we know about this cell
									bool badcell = false;
									for (int i = 0; i < worldCellList.length && !badcell; i++)
									{
										if (worldCellList.data[i].cell->formID == wcell.cell->formID)
										{
											badcell = true;
										}
									}

									if (!badcell)
									{
										worldCellList.AddItem(wcell);
									}
								}
							}
							else
							{
								_MESSAGE("unk088 is null for worldspace %08x", i);
							}
						}
					}
				}
				else
				{
					_MESSAGE("RegionList %08X is null", i);
				}
			}
		}
		_MESSAGE("worldCellList built. %i Entries", worldCellList.length);
		worldCellListBuilt = true;
		for (int i = 0; i < worldCellList.length; i++)
		{
			_MESSAGE("WorldName: %s", worldCellList.data[i].world->editorId.Get());
		}
	}

	WorldCell GetRandomWorldCell()
	{
		srand(time(NULL));
		int worldcellid = rand() % worldCellList.length;
		return worldCellList.data[worldcellid];
	}

	WorldCell GetNamedWorldCell(BSFixedString WorldspaceName)
	{
		for (int i = 0; i < worldCellList.length; i++)
		{
			if (strcmp(worldCellList.data[i].world->editorId.Get(), WorldspaceName.c_str()) == 0)
				return worldCellList.data[i];
		}

		_MESSAGE("Named World Cell not found: %s", WorldspaceName.Get());
		return WorldCell();
	}

	void MoveRefToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
	{
		_MESSAGE("Moving %08X to %08X in %s", object->formID, cell->formID, worldspace->editorId.Get());
		NiPoint3 finalPos(pos);
		MoveRef(object, cell, worldspace, finalPos,rot);
	}

	//Expensive...
	WorldCell GetWorldCellFromRef(TESObjectREFR* object)
	{
		NiPoint3 distance;
		TESObjectREFR* ref;
		int numberofRefs;
		for (int i = 0; i < worldCellList.length; i++)
		{
			numberofRefs = papyrusCell::GetNumRefs(worldCellList.data[i].cell, 0);
			for (int j = 0; j < numberofRefs; j++)
			{
				ref = papyrusCell::GetNthRef(worldCellList.data[i].cell, j, 0);
				if (ref != NULL)
				{
					distance = object->pos - ref->pos;
					Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
					if (distvector.Magnitude() < 200)
					{
						return worldCellList.data[i];
					}
				}
			}
		}
		return WorldCell();
	}

	//Interiors
/*
if (SpawnLocref == NULL)
{
	TESObjectCELL* here = GetPlayer()->parentCell;
	_MESSAGE("Here form id %08X", here->formID);
	_MESSAGE("Cell list Size %08X", GetDataHandler()->cellList.m_size);
	UInt32 cellcount = GetDataHandler()->cellList.m_size;
	for (int i = 0; i < cellcount; i++)
	{
		TESObjectCELL* parentCell = GetDataHandler()->cellList.m_data[i];
		_MESSAGE("Cell form id %08X", parentCell->formID);
		_MESSAGE("Cell form id %08X", parentCell->formID);
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("Num Ref: %i", numberofRefs);
		SpawnMonstersInCell(1, 0x06001DFC, parentCell);
	}
}
*/

}