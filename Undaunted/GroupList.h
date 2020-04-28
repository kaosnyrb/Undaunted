#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/PluginAPI.h"
namespace Undaunted
{
#ifndef GroupListdef
#define GroupListdef
	struct GroupMember {
		UInt32 FormId;
		BSFixedString BountyType;
	};

	class GroupList {
	public:
		GroupMember* data;
		int length;
		GroupList* AddItem(GroupMember item);
	};

#endif
}