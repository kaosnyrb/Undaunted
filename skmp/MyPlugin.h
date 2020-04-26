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

namespace Undaunted
{
	float MyTest(StaticFunctionTag *base);

	bool RegisterFuncs(VMClassRegistry* registry);
}
