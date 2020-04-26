#include "ConfigUtils.h"
namespace Undaunted
{
	std::string s_configPath;

	tList<UInt32> GetBadRegions() {
		tList<UInt32> list = tList<UInt32>();

		UInt32 s1 = 0x00000033;
		list.Push(&s1);

		UInt32 s2 = 0x00000036;
		list.Push(&s2);

		UInt32 s3 = 0x0000009B;
		list.Push(&s3);

		UInt32 s4 = 0x00000110;
		list.Push(&s4);

		return list;
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