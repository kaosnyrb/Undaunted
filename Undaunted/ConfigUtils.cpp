#include "ConfigUtils.h"
#include <time.h>
namespace Undaunted
{
	std::string s_configPath;
	IntList BadRegionList;
	ListLibary GroupLibary;
	UnStringList SettingsList;

	//Regions
	void AddBadRegionToConfig(UInt32 region)
	{
		_MESSAGE("Adding %08X to Bad Region List", region);
		BadRegionList.AddItem(region);
	}

	IntList GetBadRegions() {
		return BadRegionList;
	}

	//Groups
	int AddGroup(std::string questText)
	{
		_MESSAGE("Adding bounty to GroupLibary: %s", questText.c_str());
		GroupList newGroup = GroupList();
		newGroup.questText = questText;
		GroupLibary.AddItem(newGroup);
		return GroupLibary.length - 1;
	}

	void AddMembertoGroup(int id, GroupMember member)
	{
		//_MESSAGE("Adding %08X to %i of BountyType %s",member.FormId, id,member.BountyType.Get());
		GroupLibary.data[id].AddItem(member);
	}

	GroupList GetGroup(std::string bountyName)
	{
		for (int i = 0; i < GroupLibary.length; i++)
		{
			if (GroupLibary.data[i].questText.compare(bountyName) == 0)
			{
				return GroupLibary.data[i];
			}
		}
		//All else fails return something at least.
		return GetRandomGroup();
	}

	
	int count = 0;
	GroupList GetRandomGroup()
	{
		srand(time(NULL) + count++);
		int groupid = rand() % GroupLibary.length;
		_MESSAGE("Random Group: %i",groupid);
		_MESSAGE("Random Member Count: %i", GroupLibary.data[groupid].length);
		return GroupLibary.data[groupid];		
	}

	void AddConfigValue(std::string key, std::string value)
	{
		//_MESSAGE("CONFIGLENGTH: %i", SettingsList.length);
		//check if it exists		
		for (int i = 0; i < SettingsList.length; i++)
		{
			if (SettingsList.data[i].key.compare(key) == 0)
			{
				SettingsList.data[i].value = value;
				//_MESSAGE("SET: %s : %s", key, value);
				return;
			}
		}
		//doesn't exist
		UnString setting = UnString();
		setting.key = key;
		setting.value = value;
		SettingsList.AddItem(setting);
		//_MESSAGE("ADD: %s : %s", key.c_str(), value.c_str());
	}

	UInt32 GetConfigValueInt(std::string key)
	{
		for (int i = 0; i < SettingsList.length; i++)
		{
			//_MESSAGE("Comparing %s : %s", key.c_str(), SettingsList.data[i].key.c_str());
			if (SettingsList.data[i].key.compare(key) == 0)
			{
				//_MESSAGE("Found Key %s : %s", key.c_str(), SettingsList.data[i].value.c_str());
				return atoi(SettingsList.data[i].value.c_str());
			}
		}
		//Not found.
		return 0;
	}

	UnStringList RewardBlacklist = UnStringList();
	void AddRewardBlacklist(std::string key)
	{
		UnString data = UnString();
		data.value = key;
		RewardBlacklist.AddItem(data);
	}

	UnStringList getRewardBlacklist()
	{
		return RewardBlacklist;
	}

}