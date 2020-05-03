#include "LocationUtils.h"
#include <Undaunted\ConfigUtils.h>
#include "WorldCellList.h"
#include "RE/TESObjectREFR.h"
#include "RE/PlayerCharacter.h"
#include "RE/TESRegion.h"

namespace Undaunted {
	WorldCellList worldCellList;
	bool worldCellListBuilt = false;



	RE::TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID)
	{
		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		RE::TESObjectCELL* parentCell = player->parentCell;
		int numberofRefs = parentCell->objectList.size();
		_MESSAGE("GetObjectInCurrentCell Num Ref: %i", numberofRefs);
		for (int i = 0; i < numberofRefs; i++)
		{
			RE::TESObjectREFR* ref = parentCell->objectList[i];//papyrusCell::GetNthRef(parentCell, i, 0);
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

	RE::TESObjectREFR* GetRandomObjectInCell(RE::TESObjectCELL* cell)
	{
		int numberofRefs = cell->objectList.size();
		_MESSAGE("GetRandomObjectInCell Num Ref: %i", numberofRefs);		
		if (numberofRefs == 0)return NULL;
		while (true)
		{
			//Random seems to stuck sometimes. Trying to prevent that.
			int Nth = rand() % numberofRefs;
			RE::TESObjectREFR* ref = cell->objectList[Nth];
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
		DataHandler* handler = DataHandler::GetSingleton();
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
				//_MESSAGE("processing worldSpace %08X", i);
				RE::TESRegion* test = (RE::TESRegion*)handler->regionList->GetNthItem(i);
				if (test != NULL)
				{
					if (test->worldSpace == NULL)
					{
						_MESSAGE("worldSpace %08X is null", i);
					}
					else
					{
						RE::TESObjectCELL* cell = test->worldSpace->persistentCell;
						if (cell != NULL)
						{
							
							//0x00000D74
							int numberofRefs = cell->objectList.size();
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
			_MESSAGE("WorldName: %s", worldCellList.data[i].world->GetFullName());
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
			if (strcmp(worldCellList.data[i].world->GetFullName(), WorldspaceName.c_str()) == 0)
				return worldCellList.data[i];
		}

		_MESSAGE("Named World Cell not found: %s", WorldspaceName.Get());
		return WorldCell();
	}

	void MoveRefToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
	{
		_MESSAGE("Moving %08X to %08X in %s", object->formID, cell->formID, worldspace->editorId.Get());
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