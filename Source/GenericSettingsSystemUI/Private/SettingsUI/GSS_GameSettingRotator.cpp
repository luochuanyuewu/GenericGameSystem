// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingRotator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingRotator)

void UGSS_GameSettingRotator::SetDefaultOption(int32 DefaultOptionIndex)
{
	BP_OnDefaultOptionSpecified(DefaultOptionIndex);
}
