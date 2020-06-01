#include "GroupList.h"

namespace Undaunted
{
#ifndef listlibdef
#define listlibdef
	class ListLibary {
	public:
		GroupList* data;
		int length;
		ListLibary* AddItem(GroupList item);
		ListLibary* SwapItem(int first, int second);
	};
#endif
}