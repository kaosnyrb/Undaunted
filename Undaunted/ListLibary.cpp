#include "ListLibary.h"

namespace Undaunted
{
	ListLibary* ListLibary::AddItem(GroupList item)
	{
		ListLibary* currentlist = this;
		ListLibary newlist = ListLibary();
		newlist.length = currentlist->length + 1;
		newlist.data = new GroupList[newlist.length];
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