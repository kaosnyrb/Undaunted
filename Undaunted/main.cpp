#include <Undaunted\MyPlugin.h>
#include <Undaunted\BountyManager.h>
#include <Undaunted\ConfigUtils.h>
#include <Undaunted\SKSELink.h>
#include <Undaunted\StartupManager.h>
#define RUNTIME_VERSION_1_6_640	MAKE_EXE_VERSION(1, 6, 640)	// 0x01062800	the hotfix

static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;
SKSESerializationInterface* g_serialization = NULL;
SKSEMessagingInterface* g_messageInterface = NULL;

extern "C" {
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,

		1,
		"Undaunted",

		" ",
		" ",

		0,	// not version independent (extended field)
		0,	// not version independent
		{ RUNTIME_VERSION_1_6_640, 0 },	// compatible with 1.6.640

		0,	// works with any version of the script extender. you probably do not need to put anything here
	};
};


extern "C"	{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)	{	// Called by SKSE to learn about this plugin and check that it's safe to load it
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
	
		_MESSAGE("Undaunted");

		// populate info structure
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		"Undaunted";
		info->version =		1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		//else if(skse->runtimeVersion != RUNTIME_VERSION_1_5_97)
		//{
		//	_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

		//	return false;
		//}
		
		if (!Offsets::Initialize()) {
			_ERROR("Failed to load game offset database. Visit https://www.nexusmods.com/skyrimspecialedition/mods/32444 to aquire the correct database file.");			
			return false;
		}

		g_serialization = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
		g_messageInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	void SKSEMessageReceptor(SKSEMessagingInterface::Message* msg)
	{
		static bool active = true;
		if (!active)
			return;

		if (msg->type == SKSEMessagingInterface::kMessage_PreLoadGame)
		{
			//We're loading the game. Clear up any bounty data.
			_MESSAGE("kMessage_PreLoadGame rechieved, clearing bounty data.");
			if (Undaunted::BountyManager::getInstance()->activebounties.length > 0)
			{
				for (int i = 0; i < Undaunted::BountyManager::getInstance()->activebounties.length; i++)
				{
					Undaunted::BountyManager::getInstance()->ClearBountyData(i);
				}
			}
		}
		//Register to recieve interface from Enchantment Framework
		//if (msg->type == SKSEMessagingInterface::kMessage_PostLoad)


		//kMessage_InputLoaded only sent once, on initial Main Menu load
		//else if (msg->type == SKSEMessagingInterface::kMessage_InputLoaded)

	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin
		
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("Undaunted");
		_MESSAGE("Loading Undaunted..");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);
		g_messageInterface->RegisterListener(g_pluginHandle, "SKSE", SKSEMessageReceptor);
		//Check if the function registration was a success...
		bool btest = g_papyrus->Register(Undaunted::RegisterFuncs);
		

		Undaunted::GetDataHandler();
		Undaunted::GetPlayer();

		if (btest) {
			_MESSAGE("Register Succeeded");
		}

		return true;
	}
};