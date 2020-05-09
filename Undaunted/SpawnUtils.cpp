#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include "LocationUtils.h"

namespace Undaunted
{
	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry,UInt32 Type, TESObjectREFR* ref, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		NiPoint3 startingpoint = ref->pos;
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid");
			return NULL;
		}
		int spawnradius = GetConfigValueInt("BountyEnemyInteriorSpawnRadius");
		NiPoint3 offset = NiPoint3(rand() & spawnradius, rand() & spawnradius, 0);
		MoveRefToWorldCell(ref, cell, worldspace, ref->pos + offset, NiPoint3(0, 0, 0));
		TESObjectREFR* spawned = PlaceAtMe_Native(registry, 1, ref, spawnForm, 1, false, false);
		MoveRefToWorldCell(ref, cell, worldspace, startingpoint, NiPoint3(0, 0, 0));
		return spawned;
	}

	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		TESObjectREFR* spawned = NULL;
		srand(time(NULL));
		NiPoint3 startingpoint = Target->pos;
		int spawnradius = GetConfigValueInt("BountyEnemyExteriorSpawnRadius");
		for (UInt32 i = 0; i < Types.length; i++)
		{
			TESForm* spawnForm = LookupFormByID(Types.data[i].FormId);
			if (spawnForm == NULL)
			{
				_MESSAGE("Failed to Spawn. Form Invalid");
				return Types;
			}
			//If a model file path is set then change the form model.
			if (!strcmp(Types.data[i].ModelFilepath.Get(), "") == 0)
			{
				TESModel* pWorldModel = DYNAMIC_CAST(spawnForm, TESForm, TESModel);
				if (pWorldModel)
				{
					_MESSAGE("GetModelName: %s", pWorldModel->GetModelName());
					pWorldModel->SetModelName(Types.data[i].ModelFilepath.Get());
				}
			}
			if (strcmp(Types.data[i].BountyType.Get(), "Enemy") == 0 || strcmp(Types.data[i].BountyType.Get(), "Ally") == 0 || strcmp(Types.data[i].BountyType.Get(), "Placer") == 0)
			{
				//Random Offset
				NiPoint3 offset = NiPoint3(rand() & spawnradius, rand() & spawnradius, 0);
				MoveRefToWorldCell(Target, cell, worldspace, startingpoint + offset, NiPoint3(0, 0, 0));
				spawned = PlaceAtMe_Native(registry, 1, Target, spawnForm, 1, false, false);
				Types.data[i].objectRef = spawned;
				Types.data[i].isComplete = false;
			}
			else if (strcmp(Types.data[i].BountyType.Get(), "BountyDecoration") == 0 || 
				strcmp(Types.data[i].BountyType.Get(), "SpawnEffect") == 0 ||
				strcmp(Types.data[i].BountyType.Get(), "Scripted") == 0 ||
				strcmp(Types.data[i].BountyType.Get(), "ScriptedDoor") == 0)
			{
				if (spawned != NULL)
				{
					//Actors jump to the navmesh. Objects don't. This tries to used the jump to find the ground.
					TESObjectREFR* decoration = PlaceAtMe_Native(registry, 1, spawned, spawnForm, 1, false, false);
					Types.data[i].objectRef = decoration;
					Types.data[i].PreBounty();
				}
				else
				{
					TESObjectREFR* decoration = PlaceAtMe_Native(registry, 1, Target, spawnForm, 1, false, false);
					Types.data[i].objectRef = decoration;
					Types.data[i].PreBounty();
				}
			}
			else if (strcmp(Types.data[i].BountyType.Get(), "PhysicsScripted") == 0)
			{
				//We don't want these falling through the floor, so we put them in the air.
				TESObjectREFR* PhysicsScripted = PlaceAtMe_Native(registry, 1, spawned, spawnForm, 1, false, false);
				NiPoint3 offset = NiPoint3(0, 0, -1500);
				//MoveRefToWorldCell(PhysicsScripted, cell, worldspace, PhysicsScripted->pos + offset, NiPoint3(0, 0, 0));
				Types.data[i].objectRef = PhysicsScripted;
				Types.data[i].PreBounty();
			}
		}
		return Types;
	}
}