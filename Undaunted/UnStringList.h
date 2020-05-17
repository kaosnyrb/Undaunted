
namespace Undaunted
{
#ifndef UnStringListdef
#define UnStringListdef
	class UnString {
	public:
		std::string key;
		std::string value;
	};

	class UnStringList {
	public:
		UnString* data;
		int length;
		UnStringList* AddItem(UnString item);
	};
#endif
}