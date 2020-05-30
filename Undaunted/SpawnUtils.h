#include "SKSELink.h"
#include "ConfigUtils.h"
#include "RefList.h"
#include "LocationUtils.h"

namespace Undaunted
{
//	void SpawnMonsters(VMClassRegistry* registry, int count, UInt32 Type);
	TESObjectREFR* SpawnMonsterAtRef(VMClassRegistry* registry, UInt32 Type, TESObjectREFR* ref, TESObjectCELL* cell, TESWorldSpace* worldspace);
	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry, UInt32 Type, WorldCell wcell);
//	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, int count, UInt32 Type, TESObjectREFR* Target);
//	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target);
	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace);

	RefList SpawnRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace);
	VMResultArray<float> GetRiftRotations();
	RefList GetCurrentRiftRefs();
}
