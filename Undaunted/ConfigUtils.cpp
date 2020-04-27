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
		/*
		UInt32* slist;
		IntList list = IntList();
		switch (groupid)
		{
			case 0:
				_MESSAGE("Group 0");
				slist = new UInt32[5];
				slist[0] = 0x00039CFC;
				slist[1] = 0x00039CFC;
				slist[2] = 0x00039CFC;
				slist[3] = 0x00039CFC;
				slist[4] = 0x00039CFC;
				list.data = slist;
				list.length = 5;
				return list;

			case 1:
				_MESSAGE("Group 1");
				slist = new UInt32[5];
				slist[0] = 0x0003DF16;
				slist[1] = 0x0003DEC9;
				slist[2] = 0x0003DEC9;
				slist[3] = 0x00039CFC;
				slist[4] = 0x00039CFC;
				list.data = slist;
				list.length = 5;
				return list;

			case 2:
				_MESSAGE("Group 2");
				slist = new UInt32[5];
				slist[0] = 0x0003DF16;
				slist[1] = 0x0003DEC9;
				slist[2] = 0x0001E770;
				slist[3] = 0x0001E770;
				slist[4] = 0x0001E770;
				list.data = slist;
				list.length = 5;
				return list;

			case 3:
				_MESSAGE("Group 3");
				slist = new UInt32[5];
				slist[0] = 0x0003DF16;
				slist[1] = 0x0003DEC9;
				slist[2] = 0x0001E771;
				slist[3] = 0x0001E771;
				slist[4] = 0x0001E770;
				list.data = slist;
				list.length = 5;
				return list;

			default:
				_MESSAGE("Group Default");
				slist = new UInt32[5];
				slist[0] = 0x00039CFC;
				slist[1] = 0x00039CFC;
				slist[2] = 0x00039CFC;
				slist[3] = 0x00039CFC;
				slist[4] = 0x00039CFC;
				list.data = slist;
				list.length = 5;
				return list;
		}
		return IntList();
		*/
	}
}