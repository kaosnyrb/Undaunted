#include <Undaunted\ConfigUtils.h>
#include "StartupManager.h"
#include "RSJparser.tcc"
#include <filesystem>

namespace Undaunted {
	RSJresource currentfile;
	void LoadJson(const char* filepath)
	{
		_MESSAGE("Loading %s", filepath);
		std::ifstream t(filepath);
		t.seekg(0, std::ios::end);
		size_t size = t.tellg();
		std::string buffer(size, ' ');
		t.seekg(0);
		t.read(&buffer[0], size);
		RSJresource my_json(buffer.c_str());
		currentfile = my_json;
	}

	void LoadSettings()
	{
		LoadJson("Data/Undaunted/Settings.json");
		RSJresource settings = currentfile; 

		auto data = settings.as_array();
		_MESSAGE("size: %i", data.size());
		for (int i = 0; i < data.size(); i++)
		{
			auto inner = data[i].as_array();
			std::string key = inner[0].as<std::string>("default string");
			std::string value = inner[1].as<std::string>("default string");
			AddConfigValue(key.c_str(), value.c_str());
		}

		LoadJson("Data/Undaunted/RewardModBlacklist.json");
		auto modblacklist = currentfile.as_array();
		for (int i = 0; i < modblacklist.size(); i++)
		{
			std::string modname = modblacklist[i].as<std::string>("default string");
			AddRewardBlacklist(modname);
			_MESSAGE("RewardModBlacklist modname: %s", modname.c_str());
		}
	}

	void LoadGroups()
	{
		DataHandler* dataHandler = GetDataHandler();
		_MESSAGE("Loading Groups...");
		std::string path = "Data/Undaunted/Groups";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			auto filename = entry.path().u8string();
			_MESSAGE("file: %s", filename.c_str());
			if (entry.is_regular_file())
			{
				LoadJson(filename.c_str());
				RSJresource settings = currentfile;
				auto data = settings.as_array();
				_MESSAGE("size: %i", data.size());
				for (int i = 0; i < data.size(); i++)
				{
					auto group = data[i].as_array();
					std::string groupname = group[0][0].as<std::string>("groupname");
					std::string modreq = group[0][1].as<std::string>("modreq");
					int minlevel = group[0][2].as<int>(0);
					int maxlevel = group[0][3].as<int>(0);

					int groupid = AddGroup(groupname);
					for (int j = 1; j < group.size(); j++)
					{
						std::string esp = group[j][1].as<std::string>("esp");
						const ModInfo* modInfo = dataHandler->LookupModByName(esp.c_str());
						int form = group[j][2].as<int>(0);
						if (modInfo != NULL)
						{
							form = (modInfo->modIndex << 24) + form;
						}
						std::string type = group[j][3].as<std::string>("type");
						std::string model = std::string("");
						if (group[j].size() > 3)
						{
							model = group[j][4].as<std::string>("");
						}
						GroupMember newmember = GroupMember();
						newmember.BountyType = type.c_str();
						newmember.FormId = form;
						newmember.ModelFilepath = model.c_str();
						AddMembertoGroup(groupid, newmember);
					}
				}
			}
		}
	}
}
