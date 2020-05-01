#include "BountyManager.h"
#include <skse64\NiTypes.h>
#include <Undaunted\SpawnUtils.h>
#include <Undaunted\LocationUtils.h>

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
				if (distvector.Magnitude() < 5000)
				{
					for (int i = 0; i < bountygrouplist.length; i++)
					{
						_MESSAGE("Groupid : %08X ", bountygrouplist.data[i]);
					}
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


		bool alldead = true;
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "Enemy") == 0)
			{
				if (bountygrouplist.data[i].objectRef != NULL)
				{
					if (!bountygrouplist.data[i].objectRef->IsDead(1))
					{
						MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, bountygrouplist.data[i].objectRef->pos, NiPoint3(0, 0, 0));
						return false;
					}
				}
			}
		}
		//Play After Bounty Effects.
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "EndEffect") == 0)
			{
				TESForm* spawnForm = LookupFormByID(bountygrouplist.data[i].FormId);
				if (spawnForm == NULL)
				{
					_MESSAGE("Failed to Spawn. Form Invalid");
				}
				else
				{
					PlaceAtMe_Native(_registry, 1, xmarkerref, spawnForm, 1, false, false);
				}
			}
		}

		//Clean up the Decorations.
		for (UInt32 i = 0; i < bountygrouplist.length; i++)
		{
			if (strcmp(bountygrouplist.data[i].BountyType.Get(), "BountyDecoration") == 0)
			{
				MoveRefToWorldCell(bountygrouplist.data[i].objectRef, (*g_thePlayer)->parentCell, (*g_thePlayer)->currentWorldSpace,
					NiPoint3(bountygrouplist.data[i].objectRef->pos.x, bountygrouplist.data[i].objectRef->pos.y, -20000), NiPoint3(0, 0, 0));
				//bountygrouplist.data[i].objectRef->DecRef();
			}
		}
	}

	float BountyManager::StartBounty(bool nearby)
	{
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
		_MESSAGE("nearby: " + nearby);
		if (!nearby )
		{	
			bountyworldcell = GetNamedWorldCell((*g_thePlayer)->currentWorldSpace->editorId.Get());
		}
		else
		{
			bountyworldcell.cell = (*g_thePlayer)->parentCell;
			bountyworldcell.world = (*g_thePlayer)->currentWorldSpace;
		}
		_MESSAGE("target is set. Moving marker: WorldSpace: %s Cell: %08X ", bountyworldcell.world->editorId.Get(), bountyworldcell.cell->formID);
		TESObjectREFR* target = GetRandomObjectInCell(bountyworldcell.cell);
		MoveRefToWorldCell(xmarkerref, bountyworldcell.cell, bountyworldcell.world, target->pos, NiPoint3(0, 0, 0));

		bountygrouplist = GetRandomGroup();
		_MESSAGE("Setting Bounty Message: %s", bountygrouplist.questText);
		bountymessageref->fullName.name = bountygrouplist.questText;
	}

	void BountyManager::ClearBountyData() {
		bountywave = 0;
		bountygrouplist = GroupList();

		//If there's been a reload then the bounty currently breaks. Inform the user.
		if (isReady)
		{
			_MESSAGE("Setting Bounty Message: The Bounty has moved on, return to the Undaunted Camp to start a new Bounty");
			bountymessageref->fullName.name = "The Bounty has moved on, return to the Undaunted Camp to start a new Bounty";
		}
	}

}

