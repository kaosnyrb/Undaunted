#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include "LocationUtils.h"
#include "BountyManager.h"

namespace Undaunted
{
	TESObjectREFR* SpawnMonsterInCell(VMClassRegistry* registry,UInt32 Type, TESObjectREFR* ref, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		NiPoint3 startingpoint = ref->pos;
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid: %08X", Type);
			return NULL;
		}
		int spawnradius = GetConfigValueInt("BountyEnemyInteriorSpawnRadius");
		NiPoint3 offset = NiPoint3(rand() & spawnradius, rand() & spawnradius, 0);
		MoveRefToWorldCell(ref, cell, worldspace, ref->pos + offset, NiPoint3(0, 0, 0));
		TESObjectREFR* spawned = PlaceAtMe(registry, 1, ref, spawnForm, 1, true, false);
		MoveRefToWorldCell(ref, cell, worldspace, startingpoint, NiPoint3(0, 0, 0));
		return spawned;
	}

	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		TESObjectREFR* spawned = NULL;
		srand(time(NULL));
		NiPoint3 startingpoint = Target->pos;
		int spawnradius = GetConfigValueInt("BountyEnemyExteriorSpawnRadius");
		int HeightDistance = GetConfigValueInt("BountyEnemyPlacementHeightDistance");

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
			if (strcmp(Types.data[i].BountyType.c_str(), "ENEMY") == 0 || 
				strcmp(Types.data[i].BountyType.c_str(), "ALLY") == 0 || 
				strcmp(Types.data[i].BountyType.c_str(), "PLACER") == 0)
			{
				bool placedsuccessfully = false;
				int giveupcount = 20; //It's possible that we'll never find anything valid. If that's the case give up. This is quite low as we are spawning something everytime we try this.
				while (!placedsuccessfully)
				{
					//Random Offset
					NiPoint3 offset = NiPoint3(rand() & spawnradius, rand() & spawnradius, 0);
					MoveRefToWorldCell(Target, cell, worldspace, startingpoint + offset, NiPoint3(0, 0, rand() % 360));
					spawned = PlaceAtMe(registry, 1, Target, spawnForm, 1, true, false);

					int heightdist = startingpoint.z - spawned->pos.z;
					//Delete
					if ((heightdist > HeightDistance || heightdist < -HeightDistance) && giveupcount > 0)
					{
						_MESSAGE("Spawn Height is too different. Deleting.");
						MoveRefToWorldCell(spawned, cell, worldspace, NiPoint3(0, 0, 10000), NiPoint3(0, 0, 0));
						BountyManager::getInstance()->AddToDeleteList(spawned);
						giveupcount--;
					}
					else
					{
						placedsuccessfully = true;
					}
				}
				Types.data[i].objectRef = spawned;
				Types.data[i].isComplete = false;
			}
			else if (strcmp(Types.data[i].BountyType.c_str(), "BOUNTYDECORATION") == 0 ||
				strcmp(Types.data[i].BountyType.c_str(), "SPAWNEFFECT") == 0 ||
				strcmp(Types.data[i].BountyType.c_str(), "SCRIPTED") == 0 ||
				strcmp(Types.data[i].BountyType.c_str(), "SCRIPTEDDOOR") == 0)
			{
				if (spawned != NULL)
				{
					//Actors jump to the navmesh. Objects don't. This tries to used the jump to find the ground.
					TESObjectREFR* decoration = PlaceAtMe(registry, 1, spawned, spawnForm, 1, true, false);
					Types.data[i].objectRef = decoration;
					Types.data[i].isComplete = false;
					Types.data[i].PreBounty();
				}
				else
				{
					TESObjectREFR* decoration = PlaceAtMe(registry, 1, Target, spawnForm, 1, true, false);
					Types.data[i].objectRef = decoration;
					Types.data[i].isComplete = false;
					Types.data[i].PreBounty();
				}
			}
			else if (strcmp(Types.data[i].BountyType.c_str(), "PHYSICSSCRIPTED") == 0)
			{
				//We don't want these falling through the floor, so we put them in the air.
				TESObjectREFR* PhysicsScripted = PlaceAtMe(registry, 1, spawned, spawnForm, 1, true, false);
				NiPoint3 offset = NiPoint3(0, 0, -1500);
				//MoveRefToWorldCell(PhysicsScripted, cell, worldspace, PhysicsScripted->pos + offset, NiPoint3(0, 0, 0));
				Types.data[i].objectRef = PhysicsScripted;
				Types.data[i].PreBounty();
			}
		}
		return Types;
	}
}