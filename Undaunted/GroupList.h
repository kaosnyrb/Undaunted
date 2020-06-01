#include "SKSELink.h"
#include "UnStringList.h"

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
		UnStringlist Tags;
		GroupList* AddItem(GroupMember item);
		GroupList* SwapItem(int first, int second);
		void SetGroupMemberComplete(UInt32 id);
	};

#endif
}