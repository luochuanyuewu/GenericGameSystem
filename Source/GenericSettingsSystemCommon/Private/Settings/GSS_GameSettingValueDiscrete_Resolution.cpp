// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"

#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingValue.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Resolution)

FString UGSS_GameSettingValueDiscrete_Resolution::ToResolutionValue(const FIntPoint& Resolution)
{
	return FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
}

void UGSS_GameSettingValueDiscrete_Resolution::OnInitialized()
{
	RefreshResolutionOptions();
	UGSS_GameSettingValue::OnInitialized();
}

void UGSS_GameSettingValueDiscrete_Resolution::OnDependencyChanged()
{
	RefreshResolutionOptions();
	Super::OnDependencyChanged();
}

void UGSS_GameSettingValueDiscrete_Resolution::StoreInitial()
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		InitialValue = ToResolutionValue(Settings->GetScreenResolution());
		PendingValue = InitialValue;
	}
}

void UGSS_GameSettingValueDiscrete_Resolution::ResetToDefault()
{
	// UGameUserSettings has no per-platform resolution default that is safe to restore at runtime.
}

void UGSS_GameSettingValueDiscrete_Resolution::RestoreToInitial()
{
	SetValueFromString(InitialValue, EGSS_GameSettingChangeReason::RestoreToInitial);
}

bool UGSS_GameSettingValueDiscrete_Resolution::OnApply()
{
	UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	FString WidthString;
	FString HeightString;
	if (!Settings || !PendingValue.Split(TEXT("x"), &WidthString, &HeightString, ESearchCase::IgnoreCase))
	{
		return false;
	}

	const FIntPoint Resolution(FCString::Atoi(*WidthString), FCString::Atoi(*HeightString));
	if (Resolution.X <= 0 || Resolution.Y <= 0)
	{
		return false;
	}

	Settings->SetScreenResolution(Resolution);
	return true;
}

void UGSS_GameSettingValueDiscrete_Resolution::RefreshResolutionOptions()
{
	const TArray<FString> ExistingOptions = GetDynamicOptions();
	for (const FString& Option : ExistingOptions)
	{
		RemoveDynamicOption(Option);
	}

	FScreenResolutionArray AvailableResolutions;
	RHIGetAvailableResolutions(AvailableResolutions, true);
	AvailableResolutions.Sort([](const FScreenResolutionRHI& Left, const FScreenResolutionRHI& Right)
	{
		return Left.Width != Right.Width ? Left.Width < Right.Width : Left.Height < Right.Height;
	});

	TSet<FString> AddedValues;
	for (const FScreenResolutionRHI& Resolution : AvailableResolutions)
	{
		if (Resolution.Width > 0 && Resolution.Height > 0)
		{
			const FString Value = ToResolutionValue(FIntPoint(Resolution.Width, Resolution.Height));
			if (!AddedValues.Contains(Value))
			{
				AddedValues.Add(Value);
				AddDynamicOption(Value, FText::FromString(Value));
			}
		}
	}

	if (OptionValues.IsEmpty())
	{
		if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
		{
			const FString Value = ToResolutionValue(Settings->GetScreenResolution());
			AddDynamicOption(Value, FText::FromString(Value));
		}
	}

}
