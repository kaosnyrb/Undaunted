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

#include "WorldCellList.h"

namespace Undaunted
{
	TESObjectREFR* GetRefObjectInCurrentCell(UInt32 formID);
	TESObjectREFR* GetRefObjectFromWorld(UInt32 formID);
	TESObjectREFR* GetRandomObjectInCell(TESObjectCELL* cell);
	void BuildWorldList();
	WorldCell GetRandomWorldCell();
	WorldCell GetNamedWorldCell(BSFixedString WorldspaceName);
	void MoveRefToWorldCell(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot);
}