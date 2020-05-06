#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"


namespace Undaunted {
#ifndef asdasd
#define asdasd
	class BountyManager {
	public:
		static BountyManager* instance;
		static BountyManager* getInstance();

		VMClassRegistry* _registry;
		TESObjectREFR* xmarkerref = NULL;
		BGSMessage* bountymessageref = NULL;
		bool isReady = false;
		GroupList bountygrouplist;
		WorldCell bountyworldcell;
		int bountywave = 0;

		bool BountyUpdate();
		float StartBounty(bool nearby);
		void ClearBountyData();
		void SpawnBossRoomEnemies(TESObjectREFR* target);

	};
#endif
}
