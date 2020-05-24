#include "LocationUtils.h"
#include "SpawnUtils.h"
#include "ConfigUtils.h"
#include "BountyList.h"
#include "RefList.h"

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
		UnDictionary bountiesRan = UnDictionary();
		RefList deleteList = RefList();

		bool BountyUpdate(int BountyID);
		float StartBounty(int BountyID, bool nearby, const char* BountyName, TESObjectREFR* ref, BSFixedString WorldSpaceName);
		float restartBounty(int BountyID, const char* BountyName);
		void ClearBountyData(int BountyID);
		void ResetBountiesRan();
		void AddToDeleteList(TESObjectREFR* ref);
		void ClearDeleteList();
	};
#endif
}
