#include "SKSELink.h"
#include "RE/TESObjectREFR.h"

namespace Undaunted
{
#ifndef GroupListdef
#define GroupListdef
	class GroupMember {
	public:
		UInt32 FormId;
		BSFixedString BountyType;
		RE::TESObjectREFR* objectRef;

		BSFixedString ModelFilepath;

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