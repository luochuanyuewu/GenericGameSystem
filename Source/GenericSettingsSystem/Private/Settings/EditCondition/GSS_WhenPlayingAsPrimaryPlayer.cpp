// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlayingAsPrimaryPlayer.h"

#include "Engine/LocalPlayer.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

void UGSS_WhenPlayingAsPrimaryPlayer::Evaluate_Implementation(UGSS_SettingEditableState* InOutEditState)
{
	if (Setting && !Setting->GetOwningLocalPlayer()->IsPrimaryPlayer())
	{
		InOutEditState->Disable(LOCTEXT("OnlyPrimaryPlayerEditable", "Can only be changed by the primary player."));
	}
}

#undef LOCTEXT_NAMESPACE
