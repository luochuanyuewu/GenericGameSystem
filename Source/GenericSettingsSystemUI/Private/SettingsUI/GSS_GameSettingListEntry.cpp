// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingListEntry.h"

#include "CommonTextBlock.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Engine/Engine.h"
#include "Input/Events.h"
#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingListEntry)

void UGSS_GameSettingListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	NativeOnEntryReleased();
	SetSetting(Cast<UGSS_GameSetting>(ListItemObject));
}

void UGSS_GameSettingListEntry::SetSetting(UGSS_GameSetting* InSetting)
{
	Setting = InSetting;
	if (Setting)
	{
		Setting->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleEditConditionChanged);
		Setting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
		RefreshSettingName();
		HandleEditConditionChanged(Setting);
	}
	else
	{
		RefreshSettingName();
	}
}

bool UGSS_GameSettingListEntry::IsSettingEnabled() const
{
	return Setting && Setting->GetEditState().IsEnabled();
}

bool UGSS_GameSettingListEntry::IsSettingResettable() const
{
	return Setting && Setting->GetEditState().IsResettable();
}

TArray<FText> UGSS_GameSettingListEntry::GetDisabledReasons() const
{
	return Setting ? Setting->GetEditState().GetDisabledReasons() : TArray<FText>();
}

TArray<FString> UGSS_GameSettingListEntry::GetDisabledOptionValues() const
{
	return Setting ? Setting->GetEditState().GetDisabledOptions() : TArray<FString>();
}

void UGSS_GameSettingListEntry::NativeOnEntryReleased()
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

FReply UGSS_GameSettingListEntry::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
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

void UGSS_GameSettingListEntry::RefreshSettingName_Implementation()
{
}

void UGSS_GameSettingListEntry_Setting::SetSetting(UGSS_GameSetting* InSetting)
{
	Super::SetSetting(InSetting);
	RefreshSettingName();
}

void UGSS_GameSettingListEntry_Setting::RefreshSettingName_Implementation()
{
	if (Text_SettingName)
	{
		Text_SettingName->SetText(Setting ? (DisplayNameOverride.IsEmpty() ? Setting->GetDisplayName() : DisplayNameOverride) : FText::GetEmpty());
		Text_SettingName->SetVisibility(Setting ? Setting->GetDisplayNameVisibility() : ESlateVisibility::Collapsed);
	}
}

void UGSS_GameSettingListEntry::HandleSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason)
{
	if (!bSuspendChangeUpdates)
	{
		OnSettingChanged();
	}
}

void UGSS_GameSettingListEntry::OnSettingChanged()
{
}

void UGSS_GameSettingListEntry::HandleEditConditionChanged(UGSS_GameSetting*)
{
	if (Setting)
	{
		RefreshEditableState(Setting->GetEditState());
	}
}

void UGSS_GameSettingListEntry::RefreshEditableState_Implementation(const FGSS_GameSettingEditableState&)
{
}

bool UGSS_GameSettingListEntry::CanInteractWithSetting() const
{
	return IsSettingEnabled();
}

void UGSS_GameSettingListEntry::SetDisplayNameOverride(const FText& OverrideName)
{
	DisplayNameOverride = OverrideName;
	RefreshSettingName();
}
