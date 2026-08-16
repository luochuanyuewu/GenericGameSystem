// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsDetailSection.h"

#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsDetailSection)

void UGSS_SettingsDetailSection::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ReleaseSetting();
	Setting = Cast<UGSS_GameSetting>(ListItemObject);
	if (Setting)
	{
		Setting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
		Setting->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleEditStateChanged);
	}

	Super::NativeOnListItemObjectSet(ListItemObject);
	OnSettingAssigned(Setting);
}

void UGSS_SettingsDetailSection::NativeDestruct()
{
	ReleaseSetting();
	Super::NativeDestruct();
}

void UGSS_SettingsDetailSection::ReleaseSetting()
{
	if (Setting)
	{
		Setting->OnSettingChangedEvent.RemoveAll(this);
		Setting->OnSettingEditConditionChangedEvent.RemoveAll(this);
		Setting = nullptr;
	}
}

void UGSS_SettingsDetailSection::HandleSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason)
{
	OnSettingValueChanged();
}

void UGSS_SettingsDetailSection::HandleEditStateChanged(UGSS_GameSetting*)
{
	OnSettingEditableStateChanged();
}
