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
	GroupList GetGroup(const char* bountyName);

	//General
	void AddConfigValue(std::string key, std::string value);
	UInt32 GetConfigValueInt(std::string key);

}