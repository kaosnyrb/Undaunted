#include "UnStringList.h"
namespace Undaunted
{
	UnStringList* Undaunted::UnStringList::AddItem(UnString item)
	{
		UnStringList* currentlist = this;
		UnStringList newlist = UnStringList();
		newlist.length = currentlist->length + 1;
		newlist.data = new UnString[newlist.length];
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