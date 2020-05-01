#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/PluginAPI.h"
namespace Undaunted
{
#ifndef GroupListdef
#define GroupListdef
	class GroupMember {
	public:
		UInt32 FormId;
		BSFixedString BountyType;
		TESObjectREFR* objectRef;
		int IsComplete();
		bool isComplete = false;
		void PreBounty();
		void PostBounty();
	};

	class GroupList {
	public:
		const char* questText;
		GroupMember* data;
		int length;
		GroupList* AddItem(GroupMember item);
		void SetGroupMemberComplete(UInt32 id);
	};

#endif
}