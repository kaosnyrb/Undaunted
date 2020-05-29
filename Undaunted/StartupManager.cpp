#include <Undaunted\ConfigUtils.h>
#include "StartupManager.h"
#include "RSJparser.tcc"
#include <filesystem>
#include <algorithm>
#include <string>
#include <Undaunted\FormRefList.h>
#include <Undaunted\LocationUtils.h>

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

		LoadJson("Data/Undaunted/Safezones.json");
		auto Safezones = currentfile.as_array();
		for (int i = 0; i < Safezones.size(); i++)
		{
			auto obj = Safezones[i].as_object();
			std::string Zonename = obj.at("Zonename").as<std::string>("default string");
			std::string Worldspace = obj.at("Worldspace").as<std::string>("default string");
			int PosX = obj.at("PosX").as<int>();
			int PosY = obj.at("PosY").as<int>();
			int PosZ = obj.at("PosZ").as<int>();
			int Radius = obj.at("Radius").as<int>();
			Safezone zone = Safezone();
			zone.Zonename = Zonename;
			zone.Worldspace = Worldspace;
			zone.PosX = PosX;
			zone.PosY = PosY;
			zone.PosZ = PosZ;
			zone.Radius = Radius;
			AddSafezone(zone);
			_MESSAGE("Safezone %s, %s, %i, %i, %i, %i", Zonename.c_str(), Worldspace.c_str(), PosX, PosY, PosZ, Radius);
		}
	}

	void LoadRifts()
	{
		DataHandler* dataHandler = GetDataHandler();
		_MESSAGE("Loading Rifts...");
		std::string path = "Data/Undaunted/Rifts";
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
				FormRefList reflist = FormRefList();
				for (int i = 0; i < data.size(); i++)
				{
					std::string formid = data[i][0].as<std::string>("formid");

					_MESSAGE("formid: %s", formid);
					double xpos = data[i][1].as<double>();
					double ypos = data[i][2].as<double>();
					double zpos = data[i][3].as<double>();
					double xrot = data[i][4].as<double>();
					double yrot = data[i][5].as<double>();
					double zrot = data[i][6].as<double>();
					int scale = data[i][7].as<int>();

					FormRef formref = FormRef();
					if (formid.compare("RiftExit") == 0) {
						const ModInfo* modInfo = dataHandler->LookupModByName("Undaunted.esp");
						formref.formId = (modInfo->modIndex << 24) + 745728;//RiftDoor
					}
					else
					{
						formref.formId = std::stoul(formid.c_str(), nullptr, 16);
					}
					formref.pos = NiPoint3(xpos, ypos, zpos);
					formref.rot = NiPoint3(xrot, yrot, zrot);
					formref.scale = scale;
					reflist.AddItem(formref);
				}
				AddRift(reflist);
			}
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
					std::string tags = group[0][4].as<std::string>("modreq");
					_MESSAGE("tags: %s", tags.c_str());
					std::string delimiter = ",";
					size_t pos = 0;
					std::string token;
					UnStringlist taglist = UnStringlist();
					while ((pos = tags.find(delimiter)) != std::string::npos) {
						token = tags.substr(0, pos);
						std::transform(token.begin(), token.end(), token.begin(), ::toupper);
						taglist.AddItem(token);
						tags.erase(0, pos + delimiter.length());
					}
					std::transform(tags.begin(), tags.end(), tags.begin(), ::toupper);
					taglist.AddItem(tags);
					const ModInfo* modInfo = dataHandler->LookupModByName(modreq.c_str());
					if (modInfo != NULL)
					{
						int groupid = AddGroup(groupname, minlevel, maxlevel, taglist);
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
							std::transform(type.begin(), type.end(), type.begin(), ::toupper);
							GroupMember newmember = GroupMember();
							newmember.BountyType = type;
							newmember.FormId = form;
							newmember.ModelFilepath = model.c_str();
							AddMembertoGroup(groupid, newmember);
						}
					}
				}
			}
		}
		_MESSAGE("Groups loaded: %i", GetGroupCount());
	}
}
