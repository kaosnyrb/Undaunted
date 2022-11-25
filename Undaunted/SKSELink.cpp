#include "SKSELink.h"

//Big shout out to https://github.com/mwilsnd for his project https://github.com/mwilsnd/SkyrimSE-SmoothCam which helped amazingly with the address lib stuff!
DataHandler* Undaunted::GetDataHandler() {
	return *(g_dataHandler.GetPtr());
}

BSFixedString Undaunted::GetCurrentWorldspaceName()
{
	//You're thinking "Why is this here? we can already get this from the player?"
	//Answer is this call doesn't work on the VR version, so this is centralising the differences.
	return GetPlayer()->currentWorldSpace->editorId.Get();
}

PlayerCharacter* Undaunted::GetPlayer()
{
	return *g_thePlayer;
}

TESObjectREFR* Undaunted::PlaceAtMe(VMClassRegistry* registry, int count, TESObjectREFR* ref, TESForm* spawnForm, int something, bool ForcePersist, bool InitiallyDisabled)
{
	return PlaceAtMe_Native(registry, count, ref, spawnForm, 1, ForcePersist, InitiallyDisabled);
}

void Undaunted::MoveRef(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
{
	UInt32 nullHandle = *g_invalidRefHandle;
	MoveRefrToPosition(object, &nullHandle, cell, worldspace, &pos, &rot);
}