// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlatformHasTrait.h"

#include "CommonUIVisibilitySubsystem.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

TSharedRef<FGSS_WhenPlatformHasTrait> FGSS_WhenPlatformHasTrait::KillIfMissing(FGameplayTag InVisibilityTag, const FString& InKillReason)
{
	check(InVisibilityTag.IsValid());
	check(!InKillReason.IsEmpty());

	TSharedRef<FGSS_WhenPlatformHasTrait> Result = MakeShared<FGSS_WhenPlatformHasTrait>();
	Result->VisibilityTag = InVisibilityTag;
	Result->KillReason = InKillReason;
	Result->bTagDesired = true;

	return Result;
}

TSharedRef<FGSS_WhenPlatformHasTrait> FGSS_WhenPlatformHasTrait::DisableIfMissing(FGameplayTag InVisibilityTag, const FText& InDisableReason)
{
	check(InVisibilityTag.IsValid());
	check(!InDisableReason.IsEmpty());

	TSharedRef<FGSS_WhenPlatformHasTrait> Result = MakeShared<FGSS_WhenPlatformHasTrait>();
	Result->VisibilityTag = InVisibilityTag;
	Result->DisableReason = InDisableReason;
	Result->bTagDesired = true;

	return Result;
}

TSharedRef<FGSS_WhenPlatformHasTrait> FGSS_WhenPlatformHasTrait::KillIfPresent(FGameplayTag InVisibilityTag, const FString& InKillReason)
{
	check(InVisibilityTag.IsValid());
	check(!InKillReason.IsEmpty());

	TSharedRef<FGSS_WhenPlatformHasTrait> Result = MakeShared<FGSS_WhenPlatformHasTrait>();
	Result->VisibilityTag = InVisibilityTag;
	Result->KillReason = InKillReason;
	Result->bTagDesired = false;

	return Result;
}

TSharedRef<FGSS_WhenPlatformHasTrait> FGSS_WhenPlatformHasTrait::DisableIfPresent(FGameplayTag InVisibilityTag, const FText& InDisableReason)
{
	check(InVisibilityTag.IsValid());
	check(!InDisableReason.IsEmpty());

	TSharedRef<FGSS_WhenPlatformHasTrait> Result = MakeShared<FGSS_WhenPlatformHasTrait>();
	Result->VisibilityTag = InVisibilityTag;
	Result->DisableReason = InDisableReason;
	Result->bTagDesired = false;

	return Result;
}

void FGSS_WhenPlatformHasTrait::GatherEditState(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState) const
{
	if (UCommonUIVisibilitySubsystem::GetChecked(InLocalPlayer)->HasVisibilityTag(VisibilityTag) != bTagDesired)
	{
		if (KillReason.IsEmpty())
		{
			InOutEditState.Disable(DisableReason);
		}
		else
		{
			InOutEditState.Kill(KillReason);
		}
	}
}

#undef LOCTEXT_NAMESPACE
