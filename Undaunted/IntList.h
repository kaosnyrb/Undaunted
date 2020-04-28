#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/PluginAPI.h"
namespace Undaunted
{
#ifndef Intlistdef
#define Intlistdef
	class IntList {
	public:
		UInt32* data;
		int length;
		IntList* AddItem(UInt32 item);
	};

#endif
}