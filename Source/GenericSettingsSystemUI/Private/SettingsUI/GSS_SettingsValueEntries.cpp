// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsValueEntries.h"

#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingValueDiscrete.h"
#include "Settings/GSS_GameSettingValueScalar.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsValueEntries)

TArray<FText> UGSS_SettingsListEntry_Discrete::GetOptions() const
{
	if (const UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) return Discrete->GetDiscreteOptions();
	return {};
}
int32 UGSS_SettingsListEntry_Discrete::GetSelectedOptionIndex() const
{
	if (const UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) return Discrete->GetDiscreteOptionIndex();
	return INDEX_NONE;
}
bool UGSS_SettingsListEntry_Discrete::SelectOption(int32 OptionIndex)
{
	if (UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) { Discrete->SetDiscreteOptionByIndex(OptionIndex); return true; }
	return false;
}
double UGSS_SettingsListEntry_Scalar::GetNormalizedValue() const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) return Scalar->GetValueNormalized();
	return 0.0;
}
FText UGSS_SettingsListEntry_Scalar::GetFormattedValue() const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) return Scalar->GetFormattedText();
	return FText::GetEmpty();
}
bool UGSS_SettingsListEntry_Scalar::SetNormalizedValue(double Value)
{
	if (UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) { Scalar->SetValueNormalized(Value); return true; }
	return false;
}
FText UGSS_SettingsListEntry_Action::GetActionText() const
{
	if (const UGSS_GameSettingAction* Action = Cast<UGSS_GameSettingAction>(Setting)) return Action->GetActionText();
	return FText::GetEmpty();
}
bool UGSS_SettingsListEntry_Action::ExecuteAction()
{
	if (UGSS_GameSettingAction* Action = Cast<UGSS_GameSettingAction>(Setting)) { Action->ExecuteAction(); return true; }
	return false;
}
FText UGSS_SettingsListEntry_Navigation::GetNavigationText() const
{
	if (const UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting)) return Page->GetNavigationText();
	return FText::GetEmpty();
}
bool UGSS_SettingsListEntry_Navigation::Navigate()
{
	if (UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting)) { Page->ExecuteNavigation(); return true; }
	return false;
}
