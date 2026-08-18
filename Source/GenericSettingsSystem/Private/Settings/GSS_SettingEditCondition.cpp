// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingEditCondition.h"
#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingEditCondition)

void UGSS_SettingEditCondition::InitializeCondition(UGSS_GameSetting* InSetting) { Setting = InSetting; OnConditionInitialized(); }
void UGSS_SettingEditCondition::RequestRefresh() { if (Setting) Setting->RefreshEditableState(); }
void UGSS_SettingEditCondition::OnConditionInitialized_Implementation() {}
void UGSS_SettingEditCondition::OnSettingValueChanged_Implementation(EGSS_GameSettingChangeReason) {}
void UGSS_SettingEditCondition::OnSettingApplied_Implementation() {}
void UGSS_SettingEditCondition::Evaluate_Implementation(FGSS_GameSettingEditableState&) {}
