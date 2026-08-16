// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsListEntry.h"

#include "CommonTextBlock.h"
#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsListEntry)

void UGSS_SettingsListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	NativeOnEntryReleased();
	Setting = Cast<UGSS_GameSetting>(ListItemObject);
	if (Setting)
	{
		Setting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
		Setting->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleEditStateChanged);
		RefreshSettingName();
		OnSettingAssigned(Setting);
	}
	else
	{
		RefreshSettingName();
	}
}

void UGSS_SettingsListEntry::NativeOnEntryReleased()
{
	if (Setting)
	{
		Setting->OnSettingChangedEvent.RemoveAll(this);
		Setting->OnSettingEditConditionChangedEvent.RemoveAll(this);
		Setting = nullptr;
	}
	RefreshSettingName();
}

void UGSS_SettingsListEntry::RefreshSettingName()
{
	if (Text_SettingName)
	{
		Text_SettingName->SetText(Setting ? Setting->GetDisplayName() : FText::GetEmpty());
		Text_SettingName->SetVisibility(Setting ? Setting->GetDisplayNameVisibility() : ESlateVisibility::Collapsed);
	}
}

void UGSS_SettingsListEntry::HandleSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason)
{
	RefreshSettingName();
	OnSettingValueChanged();
}

void UGSS_SettingsListEntry::HandleEditStateChanged(UGSS_GameSetting*)
{
	RefreshSettingName();
	OnSettingEditableStateChanged();
}
