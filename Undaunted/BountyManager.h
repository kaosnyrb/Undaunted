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
		int isReady = 0;
		GroupList bountygrouplist;
		WorldCell bountyworldcell;
		int bountywave = 0;

		UnStringList bountiesRan = UnStringList();

		bool BountyUpdate();
		float StartBounty(bool nearby, const char* BountyName, TESObjectREFR* ref, BSFixedString WorldSpaceName);
		float restartBounty(const char* BountyName);
		void ClearBountyData();
		void ResetBountiesRan();
	};
#endif
}
