#include "BountyList.h"
namespace Undaunted
{
	BountyList* Undaunted::BountyList::AddItem(Bounty item)
	{
		BountyList* currentlist = this;
		BountyList newlist = BountyList();
		newlist.length = currentlist->length + 1;
		newlist.data = new Bounty[newlist.length];
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