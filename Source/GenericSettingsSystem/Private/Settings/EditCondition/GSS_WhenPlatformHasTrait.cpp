// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenPlatformHasTrait.h"
#include "Settings/GSS_GameSetting.h"

#include "CommonUIVisibilitySubsystem.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

UGSS_WhenPlatformHasTrait* UGSS_WhenPlatformHasTrait::KillIfMissing(UObject* Outer, FGameplayTag Tag)
{
	UGSS_WhenPlatformHasTrait* Condition = NewObject<UGSS_WhenPlatformHasTrait>(Outer);
	Condition->VisibilityTag = Tag;
	Condition->bRequireTrait = true;
	Condition->bHideWhenNotMatched = true;
	return Condition;
}

UGSS_WhenPlatformHasTrait* UGSS_WhenPlatformHasTrait::DisableIfMissing(UObject* Outer, FGameplayTag Tag, const FText& Reason)
{
	UGSS_WhenPlatformHasTrait* Condition = NewObject<UGSS_WhenPlatformHasTrait>(Outer);
	Condition->VisibilityTag = Tag;
	Condition->bRequireTrait = true;
	Condition->bHideWhenNotMatched = false;
	Condition->DisabledReason = Reason;
	return Condition;
}

UGSS_WhenPlatformHasTrait* UGSS_WhenPlatformHasTrait::KillIfPresent(UObject* Outer, FGameplayTag Tag)
{
	UGSS_WhenPlatformHasTrait* Condition = NewObject<UGSS_WhenPlatformHasTrait>(Outer);
	Condition->VisibilityTag = Tag;
	Condition->bRequireTrait = false;
	Condition->bHideWhenNotMatched = true;
	return Condition;
}

UGSS_WhenPlatformHasTrait* UGSS_WhenPlatformHasTrait::DisableIfPresent(UObject* Outer, FGameplayTag Tag, const FText& Reason)
{
	UGSS_WhenPlatformHasTrait* Condition = NewObject<UGSS_WhenPlatformHasTrait>(Outer);
	Condition->VisibilityTag = Tag;
	Condition->bRequireTrait = false;
	Condition->bHideWhenNotMatched = false;
	Condition->DisabledReason = Reason;
	return Condition;
}

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
