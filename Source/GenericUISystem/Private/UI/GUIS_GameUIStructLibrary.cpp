// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.


#include "UI/GUIS_GameUIStructLibrary.h"
#include "UI/GUIS_GameUIContext.h"
#include "UI/GUIS_GameUILayout.h"

FGUIS_UIContextBindingHandle::FGUIS_UIContextBindingHandle(ULocalPlayer* InLocalPlayer, UClass* InContextClass)
{
	LocalPlayer = InLocalPlayer;
	ContextClass = InContextClass;
}
