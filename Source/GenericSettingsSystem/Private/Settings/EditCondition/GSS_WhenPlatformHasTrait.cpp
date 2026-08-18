// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlatformHasTrait.h"
#include "Settings/GSS_GameSetting.h"

#include "CommonUIVisibilitySubsystem.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

void UGSS_WhenPlatformHasTrait::Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState)
{
	if (!Setting || UCommonUIVisibilitySubsystem::GetChecked(Setting->GetOwningLocalPlayer())->HasVisibilityTag(VisibilityTag) == bRequireTrait)
	{
		return;
	}
	if (bHideWhenNotMatched)
	{
		InOutEditState.Kill(TEXT("Platform trait does not match."));
	}
	else
	{
		InOutEditState.Disable(DisabledReason);
	}
}

#undef LOCTEXT_NAMESPACE
