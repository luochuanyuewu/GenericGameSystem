// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_OverallQuality.h"

#include "Settings/GSS_GameSettingFilterState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_OverallQuality)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscrete_OverallQuality"

void UGSS_GameSettingValueDiscrete_OverallQuality::OnInitialized()
{
	AddDynamicOption(TEXT("0"), LOCTEXT("Low", "Low"));
	AddDynamicOption(TEXT("1"), LOCTEXT("Medium", "Medium"));
	AddDynamicOption(TEXT("2"), LOCTEXT("High", "High"));
	AddDynamicOption(TEXT("3"), LOCTEXT("Epic", "Epic"));
	AddDynamicOption(TEXT("4"), LOCTEXT("Cinematic", "Cinematic"));
	AddDynamicOption(CustomValue, LOCTEXT("Custom", "Custom"));
	Super::OnInitialized();
}

int32 UGSS_GameSettingValueDiscrete_OverallQuality::GetDiscreteOptionIndex() const
{
	const int32 CurrentIndex = OptionValues.IndexOfByKey(GetValueAsString());
	return CurrentIndex != INDEX_NONE ? CurrentIndex : OptionValues.IndexOfByKey(CustomValue);
}

TArray<FText> UGSS_GameSettingValueDiscrete_OverallQuality::GetDiscreteOptions() const
{
	TArray<FText> Result = OptionDisplayTexts;
	if (GetValueAsString() != CustomValue && !Result.IsEmpty())
	{
		Result.Pop();
	}
	return Result;
}

void UGSS_GameSettingValueDiscrete_OverallQuality::SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason)
{
	if (ResolveOptionValue(InStringValue) == CustomValue)
	{
		return;
	}
	Super::SetValueFromString(InStringValue, Reason);
}

#undef LOCTEXT_NAMESPACE
