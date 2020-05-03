#include "IntList.h"
#include "ListLibary.h"
#include "UnStringList.h"

namespace Undaunted
{
	//Regions/Worldspaces
	void AddBadRegionToConfig(UInt32 region);
	IntList GetBadRegions();


	//Groups
	int AddGroup(const char* questText);
	void AddMembertoGroup(int id, GroupMember member);
	GroupList GetRandomGroup();

	//General
	void SetConfigValue(const char* key, const char* value);
	UInt32 GetConfigValueInt(const char* key);

}
