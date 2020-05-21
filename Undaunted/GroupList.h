#include "SKSELink.h"

namespace Undaunted
{
#ifndef GroupListdef
#define GroupListdef
	class GroupMember {
	public:
		UInt32 FormId;
		std::string BountyType;
		TESObjectREFR* objectRef;

		BSFixedString ModelFilepath;

		int IsComplete();
		bool isComplete = false;
		void PreBounty();
		void PostBounty();
	};

	class GroupList {
	public:
		std::string questText;
		GroupMember* data;
		UInt32 minLevel;
		UInt32 maxLevel;
		int length;
		GroupList* AddItem(GroupMember item);
		void SetGroupMemberComplete(UInt32 id);
	};

#endif
}