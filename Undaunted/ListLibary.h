#include "IntList.h"

namespace Undaunted
{
#ifndef listlibdef
#define listlibdef
	class ListLibary {
	public:
		IntList* data;
		int length;
		ListLibary* AddItem(IntList item);
	};
#endif
}