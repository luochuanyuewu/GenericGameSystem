// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsListEntry.h"

#include "CommonTextBlock.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Engine/Engine.h"
#include "Input/Events.h"
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
		HandleEditStateChanged(Setting);
	}
	else
	{
		RefreshSettingName();
	}
}

bool UGSS_SettingsListEntry::IsSettingEnabled() const
{
	return Setting && Setting->GetEditState().IsEnabled();
}

bool UGSS_SettingsListEntry::IsSettingResettable() const
{
	return Setting && Setting->GetEditState().IsResettable();
}

TArray<FText> UGSS_SettingsListEntry::GetDisabledReasons() const
{
	return Setting ? Setting->GetEditState().GetDisabledReasons() : TArray<FText>();
}

TArray<FString> UGSS_SettingsListEntry::GetDisabledOptionValues() const
{
	return Setting ? Setting->GetEditState().GetDisabledOptions() : TArray<FString>();
}

void UGSS_SettingsListEntry::NativeOnEntryReleased()
{
	StopAllAnimations();
	if (Setting)
	{
		Setting->OnSettingChangedEvent.RemoveAll(this);
		Setting->OnSettingEditConditionChangedEvent.RemoveAll(this);
		Setting = nullptr;
	}
	DisplayNameOverride = FText::GetEmpty();
	RefreshSettingName();
}

FReply UGSS_SettingsListEntry::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem(); InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* PrimaryFocusWidget = GetPrimaryGamepadFocusWidget())
		{
			if (const TSharedPtr<SWidget> CachedWidget = PrimaryFocusWidget->GetCachedWidget(); CachedWidget.IsValid())
			{
				return FReply::Handled().SetUserFocus(CachedWidget.ToSharedRef(), InFocusEvent.GetCause());
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UGSS_SettingsListEntry::RefreshSettingName()
{
	if (Text_SettingName)
	{
		Text_SettingName->SetText(Setting ? (DisplayNameOverride.IsEmpty() ? Setting->GetDisplayName() : DisplayNameOverride) : FText::GetEmpty());
		Text_SettingName->SetVisibility(Setting ? Setting->GetDisplayNameVisibility() : ESlateVisibility::Collapsed);
	}
}

void UGSS_SettingsListEntry::HandleSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason Reason)
{
	RefreshSettingName();
	OnSettingValueChanged();
	OnSettingValueChangedWithReason(Reason);
}

void UGSS_SettingsListEntry::HandleEditStateChanged(UGSS_GameSetting*)
{
	RefreshSettingName();
	OnSettingEditableStateChanged();
	OnSettingEditableStateChangedWithState(Setting ? Setting->GetEditState() : FGSS_GameSettingEditableState());
}

bool UGSS_SettingsListEntry::CanInteractWithSetting() const
{
	return IsSettingEnabled();
}

void UGSS_SettingsListEntry::SetDisplayNameOverride(const FText& OverrideName)
{
	DisplayNameOverride = OverrideName;
	RefreshSettingName();
}
