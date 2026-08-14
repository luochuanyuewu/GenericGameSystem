// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingFilterState.h"
#include "GameplayTagContainer.h"

class ULocalPlayer;

//////////////////////////////////////////////////////////////////////
// FGSS_WhenPlatformHasTrait

// Edit condition for game settings that checks CommonUI's platform traits
// to determine whether or not to show a setting
class GENERICSETTINGSSYSTEM_API FGSS_WhenPlatformHasTrait : public FGSS_GameSettingEditCondition
{
public:
	static TSharedRef<FGSS_WhenPlatformHasTrait> KillIfMissing(FGameplayTag InVisibilityTag, const FString& InKillReason);
	static TSharedRef<FGSS_WhenPlatformHasTrait> DisableIfMissing(FGameplayTag InVisibilityTag, const FText& InDisableReason);

	static TSharedRef<FGSS_WhenPlatformHasTrait> KillIfPresent(FGameplayTag InVisibilityTag, const FString& InKillReason);
	static TSharedRef<FGSS_WhenPlatformHasTrait> DisableIfPresent(FGameplayTag InVisibilityTag, const FText& InDisableReason);

	//~FGSS_GameSettingEditCondition interface
	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState) const override;
	//~End of FGSS_GameSettingEditCondition interface

private:
	FGameplayTag VisibilityTag;
	bool bTagDesired;
	FString KillReason;
	FText DisableReason;
};
