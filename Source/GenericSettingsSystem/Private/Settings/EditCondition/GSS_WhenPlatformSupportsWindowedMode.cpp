// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlatformSupportsWindowedMode.h"

#include "GenericPlatform/GenericPlatformProperties.h"

void UGSS_WhenPlatformSupportsWindowedMode::Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState)
{
	if (!FPlatformProperties::SupportsWindowedMode())
	{
		InOutEditState.Kill(TEXT("Platform does not support windowed mode."));
	}
}

UGSS_WhenPlatformSupportsWindowedMode* UGSS_WhenPlatformSupportsWindowedMode::Create(UObject* Outer)
{
	return NewObject<UGSS_WhenPlatformSupportsWindowedMode>(Outer);
}
