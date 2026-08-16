// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingEditCondition.h"
#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingEditCondition)

void UGSS_SettingEditableState::Reset()
{
	bVisible = true; bEnabled = true; bResettable = true; bHiddenFromAnalytics = false;
	DisabledOptions.Reset(); DisabledReasons.Reset();
}
void UGSS_SettingEditableState::Hide(const FString& DeveloperReason) { bVisible = false; }
void UGSS_SettingEditableState::Disable(const FText& Reason) { bEnabled = false; if (!Reason.IsEmpty()) DisabledReasons.Add(Reason); }
void UGSS_SettingEditableState::DisableOption(const FString& OptionValue) { DisabledOptions.AddUnique(OptionValue); }
void UGSS_SettingEditableState::PreventReset() { bResettable = false; }
void UGSS_SettingEditableState::HideFromAnalytics() { bHiddenFromAnalytics = true; }
void UGSS_SettingEditableState::Kill(const FString& DeveloperReason) { Hide(DeveloperReason); PreventReset(); HideFromAnalytics(); }

void UGSS_SettingEditCondition::InitializeCondition(UGSS_GameSetting* InSetting) { Setting = InSetting; OnConditionInitialized(); }
void UGSS_SettingEditCondition::RequestRefresh() { if (Setting) Setting->RefreshEditableState(); }
void UGSS_SettingEditCondition::OnConditionInitialized_Implementation() {}
void UGSS_SettingEditCondition::OnSettingValueChanged_Implementation(EGSS_GameSettingChangeReason) {}
void UGSS_SettingEditCondition::OnSettingApplied_Implementation() {}
void UGSS_SettingEditCondition::Evaluate_Implementation(UGSS_SettingEditableState*) {}
