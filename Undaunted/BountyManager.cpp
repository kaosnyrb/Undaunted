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

	float BountyManager::StartBounty(bool nearby, const char* BountyName,TESObjectREFR* ref,BSFixedString WorldSpaceName)
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
			if (ref == NULL)
			{
				_MESSAGE("ref == NULL");
				bountyworldcell = GetNamedWorldCell((*g_thePlayer)->currentWorldSpace->editorId.Get());
			}
			else
			{
				bountyworldcell = GetNamedWorldCell(WorldSpaceName.Get());
			}
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
				NiPoint3 distance;
				if (ref == NULL)
				{
					_MESSAGE("ref == NULL");
					bountyworldcell = GetNamedWorldCell((*g_thePlayer)->currentWorldSpace->editorId.Get());
					target = GetRandomObjectInCell(bountyworldcell.cell);
					distance = (*g_thePlayer)->pos - target->pos;
				}
				else
				{
					_MESSAGE("ref != NULL");
					bountyworldcell = GetNamedWorldCell(WorldSpaceName.Get());
					target = GetRandomObjectInCell(bountyworldcell.cell);
					distance = ref->pos - target->pos;
				}
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				//_MESSAGE("Distance to Bounty: %f", distvector.Magnitude());
				_MESSAGE("distance %f", distvector.Magnitude());
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

		bool foundbounty = false;
		//We do our best but if someone has ran 50 bounties without traveling there's not much we can do.
		_MESSAGE("BountyName == %s", BountyName);
		for (int i = 0; i < 50 && !foundbounty; i++)
		{
			if (_stricmp(BountyName, "") == 0)
			{
				bountygrouplist = GetRandomGroup();
			}
			else
			{
				bountygrouplist = GetGroup(BountyName);
			}
			bool bountyran = false;
			for (int j = 0; j < bountiesRan.length; j++)
			{
				if (strcmp(bountiesRan.data[j].key, bountygrouplist.questText) == 0)
				{
					bountyran = true;
				}
			}
			if (!bountyran)
			{
				foundbounty = true;
			}
		}
		UnString bountydata = UnString();
		bountydata.key = bountygrouplist.questText;
		bountiesRan.AddItem(bountydata);
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

	void BountyManager::ResetBountiesRan()
	{
		//This plays a more important role than the code suggests.
		//Micro dungeons require a cell reset to repopulate the enemies.
		//They are flagged as waiting to reset once you enter/leave them, however this only happens when the cell is unloaded.
		//Unloading the cell happens on game load OR when the player fast travels a certain distance.
		//So we watch to see when the player fast travels far enough and then allow the microdungeon back on the allowed bounty list.
		bountiesRan = UnStringList();
	}

}

