#include "MyPlugin.h"
#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"

namespace Undaunted {
	VMClassRegistry* _registry;
	TESWorldSpace* Interiors;

	TESWorldSpace* worldspace = NULL;
	TESObjectREFR* xmarkerref = NULL;
	tList<TESObjectREFR> bountyenemies;
	bool DEBUGMODE = true;

	void MoveMarker() {
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
									_MESSAGE("target is set. Moving marker", i);
									MoveMarkerToWorldCell(xmarkerref, cell, worldspace, target->pos, NiPoint3(0, 0, 0));
									return;
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
	}

	float MyTest(StaticFunctionTag* base) {
		if (xmarkerref == NULL)
		{
			_MESSAGE("NO XMARKER SET");
		}
		MoveMarker();
		_MESSAGE("Finished Regions");
		IntList group = GetRandomGroup();
		for (int i = 0; i < group.length; i++)
		{
			_MESSAGE("Groupid : %08X ", group.data[i]);
		}
		bountyenemies = SpawnMonstersAtTarget(_registry, group, xmarkerref);
		_MESSAGE("Enemy Count : %08X ", bountyenemies.Count());
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

	bool AddBadRegion(StaticFunctionTag* base, UInt32 region) {
		AddBadRegionToConfig(region);
		return true;
	}

	bool SetXMarker(StaticFunctionTag* base, TESObjectREFR* marker) {
		xmarkerref = marker;
		return true;
	}

	bool RegisterFuncs(VMClassRegistry* registry) {
		_registry = registry;
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, float>("MyTest", "MyPluginScript", Undaunted::MyTest, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, bool>("isBountyComplete", "bountyCompleteScript", Undaunted::isBountyComplete, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, UInt32>("AddBadRegion", "AddBadRegionScript", Undaunted::AddBadRegion, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, bool, TESObjectREFR*>("SetXMarker", "SetXMarkerScript", Undaunted::SetXMarker, registry));

		return true;
	}
}
