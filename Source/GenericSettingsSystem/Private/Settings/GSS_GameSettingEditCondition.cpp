// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingEditCondition.h"
#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingEditCondition)

void UGSS_GameSettingEditCondition::InitializeCondition(UGSS_GameSetting* InSetting) { Setting = InSetting; OnConditionInitialized(); }
void UGSS_GameSettingEditCondition::RequestRefresh() { if (Setting) Setting->RefreshEditableState(); }
void UGSS_GameSettingEditCondition::OnConditionInitialized_Implementation() {}
void UGSS_GameSettingEditCondition::OnSettingValueChanged_Implementation(EGSS_GameSettingChangeReason) {}
void UGSS_GameSettingEditCondition::OnSettingApplied_Implementation() {}
void UGSS_GameSettingEditCondition::Evaluate_Implementation(FGSS_GameSettingEditableState&) {}
