#include "SafezoneList.h"
namespace Undaunted
{
	SafezoneList* SafezoneList::AddItem(Safezone item)
	{
		SafezoneList* currentlist = this;
		SafezoneList newlist = SafezoneList();
		newlist.length = currentlist->length + 1;
		newlist.data = new Safezone[newlist.length];
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