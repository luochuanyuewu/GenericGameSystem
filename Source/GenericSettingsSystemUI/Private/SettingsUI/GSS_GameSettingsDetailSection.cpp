// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingsDetailSection.h"

#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsDetailSection)

void UGSS_GameSettingsDetailSection::NativeOnListItemObjectSet(UObject* ListItemObject)
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

void UGSS_GameSettingsDetailSection::NativeDestruct()
{
	ReleaseSetting();
	Super::NativeDestruct();
}

void UGSS_GameSettingsDetailSection::ReleaseSetting()
{
	if (Setting)
	{
		Setting->OnSettingChangedEvent.RemoveAll(this);
		Setting->OnSettingEditConditionChangedEvent.RemoveAll(this);
		Setting = nullptr;
	}
}

void UGSS_GameSettingsDetailSection::HandleSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason)
{
	OnSettingValueChanged();
}

void UGSS_GameSettingsDetailSection::HandleEditStateChanged(UGSS_GameSetting*)
{
	OnSettingEditableStateChanged();
}
