#include "BountyManager.h"

namespace Undaunted {
	BountyManager* BountyManager::instance = 0;
	BountyManager* BountyManager::getInstance()
	{
		if (instance == 0)
		{
			instance = new BountyManager();
		}

		return instance;
	}


	bool BountyManager::BountyUpdate()
	{
		if (bountywave == 0 && bountyworldcell.world != NULL)
		{
			//Is the player in the right worldspace?
			if (_stricmp((*g_thePlayer)->currentWorldSpace->editorId.Get(), bountyworldcell.world->editorId.Get()) == 0)
			{
				_MESSAGE("Player in Worldspace");
				//Check the distance to the XMarker
				NiPoint3 distance = (*g_thePlayer)->pos - xmarkerref->pos;
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				_MESSAGE("Distance to marker: %f", distvector.Magnitude());
				
				if (distvector.Magnitude() < GetConfigValueInt("BountyStartDistance"))
				{
					bountygrouplist = SpawnGroupAtTarget(_registry, bountygrouplist, xmarkerref, bountyworldcell.cell, bountyworldcell.world);
					_MESSAGE("Enemy Count : %08X ", bountygrouplist.length);
					bountywave = 1;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		if (bountygrouplist.length == 0)
			return false;

//		bool NoncompleteObj = false;
		int NonComplete = 0;
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			_MESSAGE("Type, Member, complete: %s, %08X , %i", bountygrouplist.data[i].BountyType.Get(), bountygrouplist.data[i].FormId, bountygrouplist.data[i].IsComplete());
			if (bountygrouplist.data[i].IsComplete() != 1)
			{
				NonComplete++;
				if (bountygrouplist.data[i].objectRef != NULL)
				{
					MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, bountygrouplist.data[i].objectRef->pos, NiPoint3(0, 0, 0));
				}
			}
		}
		if(NonComplete > 0)
			return false;

//		if (NoncompleteObj)
	//		return false;

		_MESSAGE("Starting PostBounty");
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			bountygrouplist.data[i].PostBounty();
		}
		return true;
	}

	float BountyManager::StartBounty(bool nearby)
	{
		srand(time(NULL));
		if (xmarkerref == NULL)
		{
			_MESSAGE("NO XMARKER SET");
			return 0;
		}
		if (bountymessageref == NULL)
		{
			_MESSAGE("NO BOUNTYMESSAGEREF SET");
			return 0;
		}
		if (!isReady)
		{
			_MESSAGE("System not initialised, run InitSystem before starting any bounties");
			return 0;
		}
		//Cleanup previous bounties
		ClearBountyData();

		TESObjectREFR* target = NULL;
		if (!nearby )
		{	
			bountyworldcell = GetNamedWorldCell((*g_thePlayer)->currentWorldSpace->editorId.Get());
			target = GetRandomObjectInCell(bountyworldcell.cell);
		}
		else
		{
			int loopcounts = 0;
			int BountyMinSpawnDistance = GetConfigValueInt("BountyMinSpawnDistance");
			int BountyMaxSpawnDistance = GetConfigValueInt("BountyMaxSpawnDistance");
			int BountySearchAttempts = GetConfigValueInt("BountySearchAttempts");

			bool foundtarget = false;
			while (!foundtarget)
			{
				bountyworldcell = GetNamedWorldCell((*g_thePlayer)->currentWorldSpace->editorId.Get());
				target = GetRandomObjectInCell(bountyworldcell.cell);
				NiPoint3 distance = (*g_thePlayer)->pos - target->pos;
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				//_MESSAGE("Distance to Bounty: %f", distvector.Magnitude());
				if (distvector.Magnitude() > BountyMinSpawnDistance && distvector.Magnitude() < BountyMaxSpawnDistance)
				{
					foundtarget = true;
				}
				loopcounts++;
				if (loopcounts > BountySearchAttempts)
				{
					//Can't find anything. Give up and use this cell.
					bountyworldcell.cell = (*g_thePlayer)->parentCell;
					bountyworldcell.world = (*g_thePlayer)->currentWorldSpace;
					target = GetRandomObjectInCell(bountyworldcell.cell);
					foundtarget = true;
				}
			}
		}
		_MESSAGE("target is set. Moving marker: WorldSpace: %s Cell: %08X ", bountyworldcell.world->editorId.Get(), bountyworldcell.cell->formID);
		MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, target->pos, NiPoint3(0, 0, 0));

		bountygrouplist = GetRandomGroup();
		//ClearBountyData();
		_MESSAGE("Setting Bounty Message: %s", bountygrouplist.questText);
		bountymessageref->fullName.name = bountygrouplist.questText;

		return 0;
	}

	void BountyManager::ClearBountyData() {
		for (int i = 0; i < bountygrouplist.length; i++)
		{
			//Clear all completed flags
			bountygrouplist.data[i].isComplete = false;
			bountygrouplist.data[i].objectRef = NULL;
		}
		bountywave = 0;
		bountygrouplist = GroupList();

		//If there's been a reload then the bounty currently breaks. Inform the user.
		if (isReady)
		{
			bountymessageref->fullName.name = "The Bounty has moved on, start a new Bounty";
		}
		_MESSAGE("ClearBountyData Complete");
	}

	void BountyManager::SpawnBossRoomEnemies(TESObjectREFR* target)
	{
		_MESSAGE("SpawnBossRoomEnemies");
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "BossroomEnemy") == 0)
			{
				bountygrouplist.data[i].isComplete = false;
				bountygrouplist.data[i].objectRef = SpawnMonsterInCell(_registry,bountygrouplist.data[i].FormId, target, (*g_thePlayer)->parentCell, (*g_thePlayer)->currentWorldSpace);
			}
		}

	}

}

