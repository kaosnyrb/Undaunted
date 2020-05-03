#include "SKSELink.h"
#include "RE/TESWorldSpace.h"
#include "RE/TESObjectCELL.h"
namespace Undaunted
{
#ifndef WorldCellListdef
#define WorldCellListdef
	class WorldCell {
	public:
		RE::TESWorldSpace* world;
		RE::TESObjectCELL* cell;
	};

	class WorldCellList {
	public:
		WorldCell* data;
		int length;
		WorldCellList* AddItem(WorldCell item);
	};
#endif
}