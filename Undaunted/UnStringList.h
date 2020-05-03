
namespace Undaunted
{
#ifndef UnStringListdef
#define UnStringListdef
	class UnString {
	public:
		const char* key;
		const char* value;
	};

	class UnStringList {
	public:
		UnString* data;
		int length;
		UnStringList* AddItem(UnString item);
	};
#endif
}