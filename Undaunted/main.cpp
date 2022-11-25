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
		//This function seems to be deprecated
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		"Undaunted";
		info->version =		1;
		return true;
	}

	void SKSEMessageReceptor(SKSEMessagingInterface::Message* msg)
	{
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
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin		
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Undaunted.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("Loading Undaunted..");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);
		bool btest = g_papyrus->Register(Undaunted::RegisterFuncs);

		g_serialization = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
		g_messageInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		g_messageInterface->RegisterListener(skse->GetPluginHandle(), "SKSE", SKSEMessageReceptor);

		Undaunted::GetDataHandler();
		Undaunted::GetPlayer();

		if (btest) {
			_MESSAGE("Register Succeeded");
		}
		_MESSAGE("SKSEPlugin_Load Succeeded");
		return true;
	}
};