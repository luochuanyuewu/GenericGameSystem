// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlayingAsPrimaryPlayer.h"

#include "Engine/LocalPlayer.h"
#include "Settings/GSS_GameSetting.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

UGSS_WhenPlayingAsPrimaryPlayer* UGSS_WhenPlayingAsPrimaryPlayer::Create(UObject* Outer)
{
	return NewObject<UGSS_WhenPlayingAsPrimaryPlayer>(Outer);
}

void UGSS_WhenPlayingAsPrimaryPlayer::Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState)
{
	const ULocalPlayer* LocalPlayer = Setting ? Setting->GetOwningLocalPlayer() : nullptr;
	if (LocalPlayer && !LocalPlayer->IsPrimaryPlayer())
	{
		InOutEditState.Disable(LOCTEXT("OnlyPrimaryPlayerEditable", "Can only be changed by the primary player."));
	}
}

#undef LOCTEXT_NAMESPACE
