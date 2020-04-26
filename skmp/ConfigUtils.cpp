#include "ConfigUtils.h"
namespace Undaunted
{
	std::string s_configPath;
	IntList BadRegionList;

	void AddBadRegionToConfig(UInt32 region)
	{
		_MESSAGE("Adding %08X to Bad Region List", region);
		IntList list = IntList();
		list.length = BadRegionList.length + 1;
		list.data = new UInt32[list.length];
		for (int i = 0; i < BadRegionList.length; i++)
		{
			list.data[i] = BadRegionList.data[i];
		}
		list.data[BadRegionList.length] = region;
		BadRegionList = list;
	}

	IntList GetBadRegions() {
		return BadRegionList;
	}


	IntList GetRandomGroup()
	{
		int groupid = rand() % 4;

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
	}
}