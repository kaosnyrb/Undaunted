#include "SKSELink.h"
#include "ConfigUtils.h"

namespace Undaunted
{
//	void SpawnMonsters(VMClassRegistry* registry, int count, UInt32 Type);
	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry, UInt32 Type, TESObjectCELL* parentCell);
//	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, int count, UInt32 Type, TESObjectREFR* Target);
//	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target);
	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace);
}
