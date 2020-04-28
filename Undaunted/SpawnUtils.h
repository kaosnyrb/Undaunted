#include "skse64/PapyrusForm.h"
#include "skse64/PapyrusNativeFunctions.h"

#include "skse64/GameAPI.h"
#include "skse64/GameFormComponents.h"
#include "skse64/GameForms.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameObjects.h"
#include "skse64/GameExtraData.h"
#include "skse64/PluginAPI.h"
#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/PluginAPI.h"

#include "skse64/PapyrusDefaultObjectManager.h"
#include <skse64\PapyrusCell.h>
#include <stdlib.h>
#include <skse64\PapyrusGame.h>
#include <skse64\PapyrusObjectReference.h>
#include <skse64\PapyrusObjects.h>
#include <skse64\PapyrusQuest.h>
#include "ConfigUtils.h"

namespace Undaunted
{
//	void SpawnMonsters(VMClassRegistry* registry, int count, UInt32 Type);
//	void SpawnMonstersInCell(VMClassRegistry* registry, int count, UInt32 Type, TESObjectCELL* parentCell);
//	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, int count, UInt32 Type, TESObjectREFR* Target);
	tList<TESObjectREFR> SpawnMonstersAtTarget(VMClassRegistry* registry, GroupList Types, TESObjectREFR* Target);
}
