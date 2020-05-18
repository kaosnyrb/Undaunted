#include "IntList.h"
#include "ListLibary.h"
#include "UnStringList.h"
#include "SafezoneList.h"

namespace Undaunted
{
	//Regions/Worldspaces
	void AddBadRegionToConfig(UInt32 region);
	IntList GetBadRegions();


	//Groups
	int AddGroup(std::string questText, UInt32 minlevel, UInt32 maxlevel);
	void AddMembertoGroup(int id, GroupMember member);
	GroupList GetRandomGroup();
	GroupList GetGroup(std::string bountyName);

	//General
	void AddConfigValue(std::string key, std::string value);
	UInt32 GetConfigValueInt(std::string key);
	void SetPlayerLevel(UInt32 level);
	UInt32 GetPlayerLevel();

	//RewardBlacklist
	void AddRewardBlacklist(std::string key);
	UnStringList getRewardBlacklist();

	//Safezones
	void AddSafezone(Safezone zone);
	SafezoneList GetSafezones();

}