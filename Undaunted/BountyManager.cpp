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


	bool BountyManager::BountyUpdate(int BountyID)
	{
		//The bounty list has changed. Probably from a reload.
		if (BountyID > activebounties.length)
		{
			return false;
		}
		Bounty* bounty = &activebounties.data[BountyID];
		_MESSAGE("BountyUpdate BountyID: %08X", BountyID);
		_MESSAGE("BountyID bountywave: %i", bounty->bountywave);
		if (bounty->bountywave == 0 && bounty->bountyworldcell.world != NULL)
		{
			//Is the player in the right worldspace?
			if (_stricmp(GetCurrentWorldspaceName().Get(), bounty->bountyworldcell.world->editorId.Get()) == 0)
			{
				_MESSAGE("Player in Worldspace");
				//Check the distance to the XMarker
				NiPoint3 distance = GetPlayer()->pos - bounty->xmarkerref->pos;
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				int startdis = GetConfigValueInt("BountyStartDistance");
				_MESSAGE("Distance to marker: %f / %i", distvector.Magnitude(), startdis);
				if (distvector.Magnitude() < startdis)
				{
					bounty->bountygrouplist = SpawnGroupAtTarget(_registry, bounty->bountygrouplist, bounty->xmarkerref, bounty->bountyworldcell.cell, bounty->bountyworldcell.world);
					_MESSAGE("Enemy Count : %08X ", bounty->bountygrouplist.length);
					bounty->bountywave = 1;
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

		if (bounty->bountygrouplist.length == 0)
			return false;

//		bool NoncompleteObj = false;
		int NonComplete = 0;
		for (UInt32 i = 0; i < bounty->bountygrouplist.length; i++)
		{
			_MESSAGE("Type, Member, complete: %s, %08X , %i", bounty->bountygrouplist.data[i].BountyType.Get(), bounty->bountygrouplist.data[i].FormId, bounty->bountygrouplist.data[i].IsComplete());
			if (bounty->bountygrouplist.data[i].IsComplete() != 1)
			{
				NonComplete++;
				if (bounty->bountygrouplist.data[i].objectRef != NULL)
				{
					MoveRefToWorldCell(bounty->xmarkerref, bounty->bountyworldcell.cell, bounty->bountyworldcell.world, bounty->bountygrouplist.data[i].objectRef->pos, NiPoint3(0, 0, 0));
				}
			}
		}
		if(NonComplete > 0)
			return false;

//		if (NoncompleteObj)
	//		return false;

		_MESSAGE("Starting PostBounty");
		for (UInt32 i = 0; i < bounty->bountygrouplist.length; i++)
		{
			bounty->bountygrouplist.data[i].PostBounty();
		}
		return true;
	}

	float BountyManager::StartBounty(int BountyID,bool nearby, const char* BountyName,TESObjectREFR* ref,BSFixedString WorldSpaceName)
	{
		Bounty* bounty = &activebounties.data[BountyID];
		srand(time(NULL));
		_MESSAGE("time %i", time(NULL));

		if (bounty->xmarkerref == NULL)
		{
			_MESSAGE("NO XMARKER SET");
			return 0;
		}
		if (bounty->bountymessageref == NULL)
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
		ClearBountyData(BountyID);

		TESObjectREFR* target = NULL;
		if (!nearby )
		{
			if (ref == NULL)
			{
				_MESSAGE("ref == NULL");
				bounty->bountyworldcell = GetNamedWorldCell(GetCurrentWorldspaceName().Get());
			}
			else
			{
				bounty->bountyworldcell = GetNamedWorldCell(WorldSpaceName.Get());
			}
			target = GetRandomObjectInCell(bounty->bountyworldcell);
		}
		else
		{
			int loopcounts = 0;
			int BountyMinSpawnDistance = GetConfigValueInt("BountyMinSpawnDistance");
			int BountyMaxSpawnDistance = GetConfigValueInt("BountyMaxSpawnDistance");
			int BountySearchAttempts = GetConfigValueInt("BountySearchAttempts");
			int BountyMaxHeight = GetConfigValueInt("BountyMaxHeight");
			int BountyMinHeight = GetConfigValueInt("BountyMinHeight");
			bool foundtarget = false;
			while (!foundtarget)
			{
				NiPoint3 distance;
				if (ref == NULL)
				{
					_MESSAGE("ref == NULL");
					bounty->bountyworldcell = GetNamedWorldCell(GetCurrentWorldspaceName().Get());
					target = GetRandomObjectInCell(bounty->bountyworldcell);
					distance = GetPlayer()->pos - target->pos;
				}
				else
				{
					_MESSAGE("ref != NULL ");
					_MESSAGE("WorldSpaceName: %s", WorldSpaceName.Get());
					bounty->bountyworldcell = GetNamedWorldCell(WorldSpaceName.Get());
					target = GetRandomObjectInCell(bounty->bountyworldcell);
					distance = ref->pos - target->pos;
				}
				Vector3 distvector = Vector3(distance.x, distance.y, distance.z);
				//_MESSAGE("Distance to Bounty: %f", distvector.Magnitude());
				_MESSAGE("Distance %f, Height: %f", distvector.Magnitude(), target->pos.z);
				if (distvector.Magnitude() > BountyMinSpawnDistance && 
					distvector.Magnitude() < BountyMaxSpawnDistance && 
					target->pos.z < GetPlayer()->pos.z + BountyMaxHeight &&
					target->pos.z > GetPlayer()->pos.z - BountyMinHeight)
				{
					foundtarget = true;
				}
				loopcounts++;
				if (loopcounts > BountySearchAttempts)
				{
					_MESSAGE("Can't find anything. Give up and use any cell");
					if (strcmp(WorldSpaceName.Get(), "") != 0)
					{
						bounty->bountyworldcell = GetNamedWorldCell(WorldSpaceName.Get());
					}
					else
					{
						bounty->bountyworldcell = GetNamedWorldCell(GetCurrentWorldspaceName().Get());
					}
					target = GetRandomObjectInCell(bounty->bountyworldcell);
					foundtarget = true;
				}
			}
		}
		_MESSAGE("target is set. Moving marker: WorldSpace: %s Cell: %08X ", bounty->bountyworldcell.world->editorId.Get(), bounty->bountyworldcell.cell->formID);
		MoveRefToWorldCell(bounty->xmarkerref, bounty->bountyworldcell.cell, bounty->bountyworldcell.world, target->pos, NiPoint3(0, 0, 0));

		bool foundbounty = false;
		//We do our best but if someone has ran 50 bounties without traveling there's not much we can do.
		_MESSAGE("BountyName == %s", BountyName);
		for (int i = 0; i < 50 && !foundbounty; i++)
		{
			if (_stricmp(BountyName, "") == 0)
			{
				bounty->bountygrouplist = GetRandomGroup();
			}
			else
			{
				bounty->bountygrouplist = GetGroup(std::string(BountyName));
			}
			bool bountyran = false;
			for (int j = 0; j < bountiesRan.length; j++)
			{				
				if (bountiesRan.data[j].key.compare(bounty->bountygrouplist.questText) == 0)
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
		bountydata.key = bounty->bountygrouplist.questText;
		bountiesRan.AddItem(bountydata);
		_MESSAGE("Setting Bounty Message: %s", bounty->bountygrouplist.questText.c_str());
		bounty->bountymessageref->fullName.name = bounty->bountygrouplist.questText.c_str();
		_MESSAGE("PlayerPos %f, %f, %f", GetPlayer()->pos.x, GetPlayer()->pos.y, GetPlayer()->pos.z);
		return 0;
	}

	float BountyManager::restartBounty(int BountyID,const char* BountyName)
	{
		Bounty* bounty = &activebounties.data[BountyID];
		srand(time(NULL));
		ClearBountyData(BountyID);
		bool foundbounty = false;
		bounty->bountygrouplist = GetGroup(std::string(BountyName));
		bounty->bountyworldcell = GetWorldCellFromRef(bounty->xmarkerref);
		_MESSAGE("GetWorldCellFromRef World: %s", bounty->bountyworldcell.world->editorId.Get());
		bounty->bountymessageref->fullName.name = bounty->bountygrouplist.questText.c_str();
		//BountyUpdate();
		return 0.0f;
	}

	void BountyManager::ClearBountyData(int BountyID) {
		Bounty bounty = activebounties.data[BountyID];
		for (int i = 0; i < bounty.bountygrouplist.length; i++)
		{
			//Clear all completed flags
			bounty.bountygrouplist.data[i].isComplete = false;
			bounty.bountygrouplist.data[i].objectRef = NULL;
		}
		bounty.bountywave = 0;
		bounty.bountygrouplist = GroupList();
		/*
		//If there's been a reload then the bounty currently breaks. Inform the user.
		if (isReady)
		{
			bountymessageref->fullName.name = "The Bounty has moved on, start a new Bounty";
		}*/
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

