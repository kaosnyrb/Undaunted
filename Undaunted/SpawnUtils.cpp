#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include <Undaunted\LocationUtils.h>

namespace Undaunted
{
	/*
	void SpawnMonsters(VMClassRegistry* registry, int count, UInt32 Type)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
			return;

		TESObjectCELL* parentCell = (*g_thePlayer)->parentCell;
		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		_MESSAGE("Num Ref: %i", numberofRefs);
		while (count > 0)
		{
			for (int i = 0; i < numberofRefs; i++)
			{
				TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);

				if (ref != NULL)
				{
					if (rand() % 10 + 1 > 5)
					{
						// Spawn
						TESObjectREFR* spawned = PlaceAtMe_Native(registry, 1, ref, spawnForm, 1, false, false);
						count--;
						if (spawned == NULL)
							return;
					}
				}
				if (count <= 0)
				{
					return;
				}
			}
		}
	}

	void SpawnMonstersInCell(VMClassRegistry* registry, int count, UInt32 Type, TESObjectCELL* parentCell)
	{
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
		{
			_MESSAGE("Failed to Spawn. Form Invalid");
			return;
		}

		int numberofRefs = papyrusCell::GetNumRefs(parentCell, 0);
		//No leveledChar entries. Skip
		if (numberofRefs == 0)
		{
			return;
		}
		_MESSAGE("SpawnMonstersInCell Num Ref: %i", numberofRefs);
		while (count > 0)
		{
			for (int i = 0; i < numberofRefs; i++)
			{
				TESObjectREFR* ref = papyrusCell::GetNthRef(parentCell, i, 0);

				if (ref != NULL)
				{
					if (rand() % 10 + 1 > 5)
					{
						// Spawn
						TESObjectREFR* spawned = PlaceAtMe_Native(registry, 1, ref, spawnForm, 1, false, false);
						_MESSAGE("Spawned At: %i , %i, %i", spawned->pos.x, spawned->pos.y, spawned->pos.z);
						count--;
						if (spawned == NULL)
							return;
					}
				}
				if (count <= 0)
				{
					return;
				}
			}
		}
	}

	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, int count, UInt32 Type, TESObjectREFR* Target)
	{
		tList<TESObjectREFR> outputlist = tList<TESObjectREFR>();
		TESForm* spawnForm = LookupFormByID(Type);
		if (spawnForm == NULL)
			return outputlist;

		_MESSAGE("spawntype");
		for (int i = 0; i < count; i++)
		{
			TESObjectREFR* spawned = PlaceAtMe_Native(registry, 1, Target, spawnForm, 1, false, false);
			outputlist.Push(spawned);
		}
		_MESSAGE("Target");
		return outputlist;
	}
	*/
	GroupList SpawnGroupAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target, TESObjectCELL* cell, TESWorldSpace* worldspace)
	{
		TESObjectREFR* spawned = NULL;

		for (UInt32 i = 0; i < Types.length; i++)
		{
			TESForm* spawnForm = LookupFormByID(Types.data[i].FormId);
			if (spawnForm == NULL)
			{
				_MESSAGE("Failed to Spawn. Form Invalid");
				return Types;
			}
			if (strcmp(Types.data[i].BountyType.Get(), "Enemy") == 0)
			{
				spawned = PlaceAtMe_Native(registry, 1, Target, spawnForm, 1, false, false);
				//Random Offset
				NiPoint3 offset = NiPoint3(rand() & 100, rand() & 100,0);
				MoveRefToWorldCell(spawned, cell, worldspace, spawned->pos + offset, NiPoint3(0, 0, 0));
				Types.data[i].objectRef = spawned;
			}
			else
			{
				if (spawned != NULL)
				{
					//Actors jump to the navmesh. Objects don't. This tries to used the jump to find the ground.
					TESObjectREFR* decoration = PlaceAtMe_Native(registry, 1, spawned, spawnForm, 1, false, false);
					Types.data[i].objectRef = decoration;
				}
				else
				{
					TESObjectREFR* decoration = PlaceAtMe_Native(registry, 1, Target, spawnForm, 1, false, false);
					Types.data[i].objectRef = decoration;
				}
			}
		}
		_MESSAGE("Target");
		return Types;
	}
}