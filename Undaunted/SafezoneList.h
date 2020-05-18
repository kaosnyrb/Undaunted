#include "SKSELink.h"
namespace Undaunted
{
#ifndef SafezoneListdef
#define SafezoneListdef
	class Safezone {
	public:
		std::string Zonename;
		std::string Worldspace;
		int PosX;
		int PosY;
		int PosZ;
		int Radius;
	};

	class SafezoneList {
	public:
		Safezone* data;
		int length;
		SafezoneList* AddItem(Safezone item);
	};
#endif
}