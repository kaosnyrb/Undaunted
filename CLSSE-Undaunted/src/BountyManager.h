#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include "SKSELink.h"

namespace Undaunted {
#ifndef asdasd
#define asdasd
	class BountyManager {
	public:
		static BountyManager* instance;
		static BountyManager* getInstance();

		VMClassRegistry* _registry;
		RE::TESObjectREFR* xmarkerref = NULL;
		RE::BGSMessage* bountymessageref = NULL;
		bool isReady = false;
		GroupList bountygrouplist;
		WorldCell bountyworldcell;
		int bountywave = 0;

		bool BountyUpdate();
		float StartBounty(bool nearby);
		void ClearBountyData();


	};
#endif
}
