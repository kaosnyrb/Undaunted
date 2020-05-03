#include "SKSELink.h"
#include "ConfigUtils.h"

namespace Undaunted
{
//	void SpawnMonsters(VMClassRegistry* registry, int count, UInt32 Type);
//	void SpawnMonstersInCell(VMClassRegistry* registry, int count, UInt32 Type, TESObjectCELL* parentCell);
//	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, int count, UInt32 Type, TESObjectREFR* Target);
//	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target);
	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, RE::TESObjectREFR* Target, RE::TESObjectCELL* cell, RE::TESWorldSpace* worldspace);
}
