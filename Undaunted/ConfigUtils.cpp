#include "ConfigUtils.h"
namespace Undaunted
{
	std::string s_configPath;
	IntList BadRegionList;
	ListLibary GroupLibary;

	//Regions
	void AddBadRegionToConfig(UInt32 region)
	{
		_MESSAGE("Adding %08X to Bad Region List", region);
		BadRegionList.AddItem(region);
	}

	IntList GetBadRegions() {
		return BadRegionList;
	}

	void GetWorldWhitelist(UnString region)
	{
	}

	UnStringList AddWorldToWhitelist(UnString region)
	{
		return UnStringList();
	}

	//Groups
	int AddGroup()
	{
		_MESSAGE("Adding to GroupLibary");
		GroupLibary.AddItem(IntList());
		return GroupLibary.length - 1;
	}

	void AddMembertoGroup(int id, UInt32 member)
	{
		_MESSAGE("Adding %08X to %i",member, id);
		GroupLibary.data[id].AddItem(member);
	}

	IntList GetRandomGroup()
	{
		int groupid = rand() % GroupLibary.length;
		_MESSAGE("Random Group: %i",groupid);
		_MESSAGE("Random Member Count: %i", GroupLibary.data[groupid].length);
		return GroupLibary.data[groupid];		
	}
}