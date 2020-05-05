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
	int AddGroup(const char* questText)
	{
		_MESSAGE("Adding bounty to GroupLibary: %s", questText);
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

	GroupList GetRandomGroup()
	{
		srand(time(NULL));
		int groupid = rand() % GroupLibary.length;
		_MESSAGE("Random Group: %i",groupid);
		_MESSAGE("Random Member Count: %i", GroupLibary.data[groupid].length);
		return GroupLibary.data[groupid];		
	}

	void SetConfigValue(const char* key, const char* value)
	{
		UnString setting = UnString();
		setting.key = key;
		setting.value = value;
		SettingsList.AddItem(setting);
		//_MESSAGE("%s : %s", key, value);
	}

	UInt32 GetConfigValueInt(const char* key)
	{
		for (int i = 0; i < SettingsList.length; i++)
		{
			if (strcmp(SettingsList.data[i].key, key) == 0)
			{
				//_MESSAGE("Found Key %s : %s", key, SettingsList.data[i].value);
				return atoi(SettingsList.data[i].value);
			}
		}
		//Not found.
		return 0;
	}
}