// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlayingAsPrimaryPlayer.h"

#include "Engine/LocalPlayer.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

TSharedRef<FGSS_WhenPlayingAsPrimaryPlayer> FGSS_WhenPlayingAsPrimaryPlayer::Get()
{
	static TSharedRef<FGSS_WhenPlayingAsPrimaryPlayer> Instance = MakeShared<FGSS_WhenPlayingAsPrimaryPlayer>();
	return Instance;
}

void FGSS_WhenPlayingAsPrimaryPlayer::GatherEditState(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState) const
{
	if (!InLocalPlayer->IsPrimaryPlayer())
	{
		InOutEditState.Disable(LOCTEXT("OnlyPrimaryPlayerEditable", "Can only be changed by the primary player."));
	}
}

#undef LOCTEXT_NAMESPACE
