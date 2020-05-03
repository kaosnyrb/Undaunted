#include "WorldCellList.h"
#include "RE/TESObjectREFR.h"

namespace Undaunted
{
	RE::TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID);
	//TESObjectREFR* GetRefObjectFromWorld(UInt32 formID);
	RE::TESObjectREFR* GetRandomObjectInCell(RE::TESObjectCELL* cell);
	void BuildWorldList();
	WorldCell GetRandomWorldCell();
	WorldCell GetNamedWorldCell(RE::BSFixedString WorldspaceName);
	void MoveRefToWorldCell(RE::TESObjectREFR* object, RE::TESObjectCELL* cell, RE::TESWorldSpace* worldspace, RE::NiPoint3 pos, RE::NiPoint3 rot);
}