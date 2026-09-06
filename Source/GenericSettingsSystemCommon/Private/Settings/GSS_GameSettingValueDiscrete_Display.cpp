// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Display.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingFilterState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Display)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscrete_Display"

void UGSS_GameSettingValueDiscrete_Display::BeginDestroy()
{
	UnbindDisplayListeners();
	Super::BeginDestroy();
}

void UGSS_GameSettingValueDiscrete_Display::OnInitialized()
{
	Super::OnInitialized();
	BindDisplayListeners();
}

void UGSS_GameSettingValueDiscrete_Display::StoreInitial()
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		InitialMonitorID = Settings->GetDisplayID();
		InitialMonitorIndex = Settings->GetDisplayIndex();
	}
}

void UGSS_GameSettingValueDiscrete_Display::ResetToDefault()
{
}

void UGSS_GameSettingValueDiscrete_Display::RestoreToInitial()
{
	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		Settings->SetDisplayProperties(InitialMonitorID, InitialMonitorIndex);
		NotifySettingChanged(EGSS_GameSettingChangeReason::RestoreToInitial);
	}
}

void UGSS_GameSettingValueDiscrete_Display::SetDiscreteOptionByIndex(int32 Index)
{
	SetDisplayFromIndex(Index, EGSS_GameSettingChangeReason::Change);
}

int32 UGSS_GameSettingValueDiscrete_Display::GetDiscreteOptionIndex() const
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		return CurrentDisplayMetrics.GetClosestMonitorFromIDAndIndex(Settings->GetDisplayID(), Settings->GetDisplayIndex());
	}
	return INDEX_NONE;
}

int32 UGSS_GameSettingValueDiscrete_Display::GetDiscreteOptionDefaultIndex() const
{
	return CurrentDisplayMetrics.GetClosestMonitorFromIDAndIndex(InitialMonitorID, InitialMonitorIndex);
}

TArray<FText> UGSS_GameSettingValueDiscrete_Display::GetDiscreteOptions() const
{
	TArray<FText> Options;
	const FText UnknownDisplayText = LOCTEXT("UnknownDisplay", "[Unknown]");
	if (CurrentDisplayMetrics.MonitorInfo.IsEmpty())
	{
		Options.Add(UnknownDisplayText);
		return Options;
	}

	for (const FMonitorInfo& Monitor : CurrentDisplayMetrics.MonitorInfo)
	{
		if (!Monitor.FriendlyName.IsEmpty())
		{
			Options.Add(FText::FromString(Monitor.FriendlyName));
		}
		else if (!Monitor.Name.IsEmpty())
		{
			Options.Add(FText::FromString(Monitor.Name));
		}
		else
		{
			Options.Add(UnknownDisplayText);
		}
	}
	return Options;
}

void UGSS_GameSettingValueDiscrete_Display::OnDependencyChanged()
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		SetDisplayFromIndex(CurrentDisplayMetrics.GetClosestMonitorFromIDAndIndex(Settings->GetDisplayID(), Settings->GetDisplayIndex()), EGSS_GameSettingChangeReason::DependencyChanged);
	}
}

void UGSS_GameSettingValueDiscrete_Display::BindDisplayListeners()
{
	if (FSlateApplication::IsInitialized())
	{
		if (const TSharedPtr<GenericApplication> PlatformApplication = FSlateApplication::Get().GetPlatformApplication())
		{
			FDisplayMetrics::RebuildDisplayMetrics(CurrentDisplayMetrics);
			GenericApplication::FOnDisplayMetricsChanged& DisplayMetricsChangedEvent = PlatformApplication->OnDisplayMetricsChanged();
			if (!DisplayMetricsChangedEvent.IsBoundToObject(this))
			{
				DisplayMetricsChangedHandle = DisplayMetricsChangedEvent.AddUObject(this, &ThisClass::OnDisplayMetricsChanged);
			}
		}
	}

	if (const ULocalPlayer* OwningPlayer = GetOwningLocalPlayer())
	{
		if (UGameViewportClient* ViewportClient = OwningPlayer->ViewportClient)
		{
			if (!ViewportClient->OnWindowDisplayChanged().IsBoundToObject(this))
			{
				WindowDisplayChangedHandle = ViewportClient->OnWindowDisplayChanged().AddUObject(this, &ThisClass::HandleExternalDisplayChanged);
			}
			if (!ViewportClient->OnToggleFullscreen().IsBoundToObject(this))
			{
				ToggleFullscreenHandle = ViewportClient->OnToggleFullscreen().AddUObject(this, &ThisClass::HandleToggleFullscreen);
			}
		}
	}

	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		if (!Settings->OnGameUserSettingsVideoRevert.IsBoundToObject(this))
		{
			VideoRevertHandle = Settings->OnGameUserSettingsVideoRevert.AddUObject(this, &ThisClass::HandleExternalDisplayChanged);
		}
	}
}

void UGSS_GameSettingValueDiscrete_Display::UnbindDisplayListeners()
{
	if (FSlateApplication::IsInitialized())
	{
		if (const TSharedPtr<GenericApplication> PlatformApplication = FSlateApplication::Get().GetPlatformApplication())
		{
			PlatformApplication->OnDisplayMetricsChanged().Remove(DisplayMetricsChangedHandle);
		}
	}
	DisplayMetricsChangedHandle.Reset();

	if (const ULocalPlayer* OwningPlayer = GetOwningLocalPlayer())
	{
		if (UGameViewportClient* ViewportClient = OwningPlayer->ViewportClient)
		{
			ViewportClient->OnWindowDisplayChanged().Remove(WindowDisplayChangedHandle);
			ViewportClient->OnToggleFullscreen().Remove(ToggleFullscreenHandle);
		}
	}
	WindowDisplayChangedHandle.Reset();
	ToggleFullscreenHandle.Reset();

	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		Settings->OnGameUserSettingsVideoRevert.Remove(VideoRevertHandle);
	}
	VideoRevertHandle.Reset();
}

void UGSS_GameSettingValueDiscrete_Display::OnDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetrics)
{
	CurrentDisplayMetrics = NewDisplayMetrics;
	RefreshEditableState();
}

void UGSS_GameSettingValueDiscrete_Display::HandleExternalDisplayChanged()
{
	RefreshEditableState();
}

void UGSS_GameSettingValueDiscrete_Display::HandleToggleFullscreen(bool)
{
	HandleExternalDisplayChanged();
}

void UGSS_GameSettingValueDiscrete_Display::SetDisplayFromIndex(int32 Index, EGSS_GameSettingChangeReason Reason)
{
	UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (Settings && CurrentDisplayMetrics.MonitorInfo.IsValidIndex(Index))
	{
		Settings->SetDisplayProperties(CurrentDisplayMetrics.MonitorInfo[Index].ID, Index);
	}
	NotifySettingChanged(Reason);
}

#undef LOCTEXT_NAMESPACE
