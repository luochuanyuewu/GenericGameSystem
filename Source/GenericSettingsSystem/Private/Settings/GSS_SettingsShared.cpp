// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#include "Settings/GSS_SettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsShared)

void UGSS_SettingsShared::ApplySettings_Implementation()
{
}

void UGSS_SettingsShared::SaveSettings_Implementation()
{
	AsyncSaveGameToSlotForLocalPlayer();
}
