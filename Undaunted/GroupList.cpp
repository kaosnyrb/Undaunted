#include "IntList.h"
#include "GroupList.h"
#include <Undaunted\LocationUtils.h>
#include <Undaunted\BountyManager.h>

namespace Undaunted
{
	GroupList* GroupList::AddItem(GroupMember item)
	{
		GroupList* currentlist = this;
		GroupList newlist = GroupList();
		newlist.length = currentlist->length + 1;
		newlist.data = new GroupMember[newlist.length];
		for (int i = 0; i < currentlist->length; i++)
		{
			newlist.data[i] = currentlist->data[i];
		}
		newlist.data[currentlist->length] = item;
		currentlist->data = newlist.data;
		currentlist->length = newlist.length;
		return currentlist;
	}

	void GroupList::SetGroupMemberComplete(UInt32 id)
	{
		for (int i = 0; i < this->length; i++)
		{
			if ( id == this->data[i].objectRef->formID)
			{
				this->data[i].isComplete = true;
				_MESSAGE("SetGroupMemberComplete: %08X ", id);
				return;
			}
		}
	}

	int GroupMember::IsComplete()
	{
		const char* type = this->BountyType.Get();
		if (strcmp(type, "PhysicsScripted") == 0 || strcmp(type, "Scripted") == 0 || strcmp(type, "Enemy") == 0)
		{
			if (isComplete)
			{
				return 1;
			}
		}
		if (strcmp(type, "BountyDecoration") == 0 || strcmp(type,"EndEffect") == 0 || strcmp(type, "SpawnEffect") == 0 || strcmp(type, "Ally") == 0)
		{
			return 1;
		}
		return 0;
	}

	void GroupMember::PreBounty()
	{

	}

	void GroupMember::PostBounty()
	{
		_MESSAGE("PostBounty");
		const char* type = this->BountyType.Get();
		if (strcmp(type, "EndEffect") == 0)
		{
			TESForm* spawnForm = LookupFormByID(this->FormId);
			if (spawnForm == NULL)
			{
				_MESSAGE("Failed to Spawn. Form Invalid");
			}
			else
			{
				PlaceAtMe_Native(BountyManager::getInstance()->_registry, 1, BountyManager::getInstance()->xmarkerref, spawnForm, 1, false, false);
			}
		}
		/*
		if (strcmp(type, "BountyDecoration") == 0)
		{
			MoveRefToWorldCell(this->objectRef, (*g_thePlayer)->parentCell, (*g_thePlayer)->currentWorldSpace,
				NiPoint3(this->objectRef->pos.x, this->objectRef->pos.y, -20000), NiPoint3(0, 0, 0));
		}*/
	}
}