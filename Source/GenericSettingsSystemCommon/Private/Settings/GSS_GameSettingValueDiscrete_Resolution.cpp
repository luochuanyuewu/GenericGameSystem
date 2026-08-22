// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"

#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingFilterState.h"
#include "Settings/GSS_GameSettingValue.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Resolution)

FString UGSS_GameSettingValueDiscrete_Resolution::ToResolutionValue(const FIntPoint& Resolution)
{
	return FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
}

void UGSS_GameSettingValueDiscrete_Resolution::OnInitialized()
{
	RefreshResolutionOptions();
	Super::OnInitialized();
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
	}
}

void UGSS_GameSettingValueDiscrete_Resolution::ResetToDefault()
{
	// UGameUserSettings has no per-platform resolution default that is safe to restore at runtime.
}

void UGSS_GameSettingValueDiscrete_Resolution::RestoreToInitial()
{
	SetResolutionFromValue(InitialValue, EGSS_GameSettingChangeReason::RestoreToInitial);
}

void UGSS_GameSettingValueDiscrete_Resolution::SetDiscreteOptionByIndex(int32 Index)
{
	if (OptionValues.IsValidIndex(Index))
	{
		SetResolutionFromValue(OptionValues[Index], EGSS_GameSettingChangeReason::Change);
	}
}

int32 UGSS_GameSettingValueDiscrete_Resolution::GetDiscreteOptionIndex() const
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		const int32 MatchingIndex = FindOptionIndex(ToResolutionValue(Settings->GetScreenResolution()));
		if (MatchingIndex != INDEX_NONE)
		{
			return MatchingIndex;
		}
	}
	return OptionValues.IsEmpty() ? INDEX_NONE : OptionValues.Num() - 1;
}

TArray<FText> UGSS_GameSettingValueDiscrete_Resolution::GetDiscreteOptions() const
{
	return OptionDisplayTexts;
}

void UGSS_GameSettingValueDiscrete_Resolution::SetResolutionFromValue(const FString& Value, EGSS_GameSettingChangeReason Reason)
{
	UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	FString WidthString;
	FString HeightString;
	if (Settings && Value.Split(TEXT("x"), &WidthString, &HeightString, ESearchCase::IgnoreCase))
	{
		const FIntPoint Resolution(FCString::Atoi(*WidthString), FCString::Atoi(*HeightString));
		if (Resolution.X > 0 && Resolution.Y > 0)
		{
			Settings->SetScreenResolution(Resolution);
		}
	}

	NotifySettingChanged(Reason);
}

int32 UGSS_GameSettingValueDiscrete_Resolution::FindOptionIndex(const FString& Value) const
{
	return OptionValues.IndexOfByKey(Value);
}

void UGSS_GameSettingValueDiscrete_Resolution::RefreshResolutionOptions()
{
	OptionValues.Reset();
	OptionDisplayTexts.Reset();

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
				OptionValues.Add(Value);
				OptionDisplayTexts.Add(FText::FromString(Value));
			}
		}
	}

	if (OptionValues.IsEmpty())
	{
		if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
		{
			const FString Value = ToResolutionValue(Settings->GetScreenResolution());
			OptionValues.Add(Value);
			OptionDisplayTexts.Add(FText::FromString(Value));
		}
	}
}
