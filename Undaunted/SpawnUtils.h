#include "SKSELink.h"
#include "ConfigUtils.h"
#include "RefList.h"
#include "LocationUtils.h"

namespace Undaunted
{
	TESObjectREFR* SpawnMonsterAtRef(VMClassRegistry* registry, UInt32 Type, TESObjectREFR* ref, TESObjectCELL* cell, TESWorldSpace* worldspace);
	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry, UInt32 Type, WorldCell wcell);
	GroupList SpawnGroupInCell(VMClassRegistry* registry, GroupList Types, WorldCell wcell);
	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace, int spawnradius, int HeightDistance);
	RefList SpawnRift(VMClassRegistry* registry, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace);
	VMResultArray<float> GetRiftRotations();
	RefList GetCurrentRiftRefs();
}
