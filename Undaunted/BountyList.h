#include "SKSELink.h"
#include <Undaunted\WorldCellList.h>
#include <Undaunted\GroupList.h>

namespace Undaunted
{
#ifndef BountyListdef
#define BountyListdef
	class Bounty {
	public:
		TESObjectREFR* xmarkerref = NULL;
		BGSMessage* bountymessageref = NULL;
		GroupList bountygrouplist;
		WorldCell bountyworldcell;
		int bountywave = 0;
	};

	class BountyList {
	public:
		Bounty* data;
		int length;
		BountyList* AddItem(Bounty item);
	};
#endif
}