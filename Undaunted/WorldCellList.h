#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/PluginAPI.h"

namespace Undaunted
{
#ifndef WorldCellListdef
#define WorldCellListdef
	class WorldCell {
	public:
		TESWorldSpace* world;
		TESObjectCELL* cell;
	};

	class WorldCellList {
	public:
		WorldCell* data;
		int length;
		WorldCellList* AddItem(WorldCell item);
	};
#endif
}