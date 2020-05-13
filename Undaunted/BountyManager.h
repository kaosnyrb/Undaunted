#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include "BountyList.h"

namespace Undaunted {
#ifndef BountyManagerdef
#define BountyManagerdef
	class BountyManager {
	public:
		static BountyManager* instance;
		static BountyManager* getInstance();

		VMClassRegistry* _registry;
		int isReady = 0;

		BountyList activebounties = BountyList();
		UnStringList bountiesRan = UnStringList();

		bool BountyUpdate(int BountyID);
		float StartBounty(int BountyID, bool nearby, const char* BountyName, TESObjectREFR* ref, BSFixedString WorldSpaceName);
		float restartBounty(int BountyID, const char* BountyName);
		void ClearBountyData(int BountyID);
		void ResetBountiesRan();
	};
#endif
}
