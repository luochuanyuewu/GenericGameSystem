// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingsDeveloperSettings.h"

#include "Settings/GSS_SettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsDeveloperSettings)

UClass* UGSS_SettingsDeveloperSettings::ResolveSharedSettingsClass() const
{
	if (UClass* LoadedClass = SharedSettingsClass.LoadSynchronous())
	{
		return LoadedClass;
	}
	return UGSS_SettingsShared::StaticClass();
}
