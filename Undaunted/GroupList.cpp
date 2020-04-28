#include "IntList.h"
#include "GroupList.h"

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
}