// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscreteDynamic)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscreteDynamic"

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic::UGSS_GameSettingValueDiscreteDynamic()
{
}

void UGSS_GameSettingValueDiscreteDynamic::SetAccessor(const FGSS_SettingValueAccessor& InAccessor)
{
	Accessor = InAccessor;
}

void UGSS_GameSettingValueDiscreteDynamic::SetDefaultValueFromString(FString InOptionValue)
{
	DefaultValue = InOptionValue;
}

void UGSS_GameSettingValueDiscreteDynamic::AddDynamicOption(FString InOptionValue, FText InOptionText)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!OptionValues.Contains(InOptionValue), TEXT("You already added this option InOptionValue: %s InOptionText %s."), *InOptionValue, *InOptionText.ToString());
#endif

	OptionValues.Add(InOptionValue);
	OptionDisplayTexts.Add(InOptionText);
}

void UGSS_GameSettingValueDiscreteDynamic::RemoveDynamicOption(FString InOptionValue)
{
	const int32 Index = OptionValues.IndexOfByKey(InOptionValue);
	if (Index != INDEX_NONE)
	{
		OptionValues.RemoveAt(Index);
		OptionDisplayTexts.RemoveAt(Index);
	}
}

const TArray<FString>& UGSS_GameSettingValueDiscreteDynamic::GetDynamicOptions()
{
	return OptionValues;
}

bool UGSS_GameSettingValueDiscreteDynamic::HasDynamicOption(const FString& InOptionValue)
{
	return OptionValues.ContainsByPredicate([this, &InOptionValue](const FString& Option)
	{
		return AreOptionsEqual(InOptionValue, Option);
	});
}

FString UGSS_GameSettingValueDiscreteDynamic::GetValueAsString() const
{
	return PendingValue;
}

void UGSS_GameSettingValueDiscreteDynamic::SetValueFromString(FString InStringValue)
{
	SetValueFromString(InStringValue, EGSS_GameSettingChangeReason::Change);
}

void UGSS_GameSettingValueDiscreteDynamic::SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason)
{
	if (const int32 MatchingIndex = OptionValues.IndexOfByPredicate([&InStringValue, this](const FString& Option)
		{
			return AreOptionsEqual(InStringValue, Option);
		});
		MatchingIndex != INDEX_NONE)
	{
		InStringValue = OptionValues[MatchingIndex];
	}

	PendingValue = InStringValue;
	NotifySettingChanged(Reason);
}

bool UGSS_GameSettingValueDiscreteDynamic::AreOptionsEqual(const FString& InOptionA, const FString& InOptionB) const
{
	return FGSS_SettingValueAccessor::AreSerializedValuesEqual(InOptionA, InOptionB);
}

void UGSS_GameSettingValueDiscreteDynamic::OnInitialized()
{
	Super::OnInitialized();
}

void UGSS_GameSettingValueDiscreteDynamic::StoreInitial()
{
	FString AppliedValue;
	if (Accessor.GetValue(LocalPlayer, AppliedValue))
	{
		InitialValue = AppliedValue;
	}
	else if (DefaultValue.IsSet())
	{
		InitialValue = DefaultValue.GetValue();
	}

	if (const int32 MatchingIndex = OptionValues.IndexOfByPredicate([this](const FString& Option)
		{
			return AreOptionsEqual(InitialValue, Option);
		});
		MatchingIndex != INDEX_NONE)
	{
		InitialValue = OptionValues[MatchingIndex];
	}

	PendingValue = InitialValue;
}

void UGSS_GameSettingValueDiscreteDynamic::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValueFromString(DefaultValue.GetValue(), EGSS_GameSettingChangeReason::ResetToDefault);
	}
}

void UGSS_GameSettingValueDiscreteDynamic::RestoreToInitial()
{
	SetValueFromString(InitialValue, EGSS_GameSettingChangeReason::RestoreToInitial);
}

bool UGSS_GameSettingValueDiscreteDynamic::OnApply()
{
	return Accessor.SetValue(LocalPlayer, PendingValue);
}

void UGSS_GameSettingValueDiscreteDynamic::SetDiscreteOptionByIndex(int32 Index)
{
	const TArray<int32> EnabledIndices = GetEnabledOptionIndices();
	if (ensure(EnabledIndices.IsValidIndex(Index)))
	{
		SetValueFromString(OptionValues[EnabledIndices[Index]]);
	}
}

int32 UGSS_GameSettingValueDiscreteDynamic::GetDiscreteOptionIndex() const
{
	const FString CurrentValue = GetValueAsString();
	const int32 Index = OptionValues.IndexOfByPredicate([this, CurrentValue](const FString& InOption) {
		return AreOptionsEqual(CurrentValue, InOption);
	});

	// If we can't find the correct index, send the default index.
	if (Index == INDEX_NONE)
	{
		return GetDiscreteOptionDefaultIndex();
	}

	return GetEnabledOptionIndex(Index);
}

int32 UGSS_GameSettingValueDiscreteDynamic::GetDiscreteOptionDefaultIndex() const
{
	if (DefaultValue.IsSet())
	{
		const int32 RawIndex = OptionValues.IndexOfByPredicate([this](const FString& InOption) {
			return AreOptionsEqual(DefaultValue.GetValue(), InOption);
		});
		return GetEnabledOptionIndex(RawIndex);
	}

	return INDEX_NONE;
}

TArray<FText> UGSS_GameSettingValueDiscreteDynamic::GetDiscreteOptions() const
{
	const TArray<int32> EnabledIndices = GetEnabledOptionIndices();
	if (EnabledIndices.Num() != OptionDisplayTexts.Num())
	{
		TArray<FText> AllowedOptions;
		AllowedOptions.Reserve(EnabledIndices.Num());
		for (const int32 OptionIndex : EnabledIndices)
		{
			AllowedOptions.Add(OptionDisplayTexts[OptionIndex]);
		}
		return AllowedOptions;
	}

	return OptionDisplayTexts;
}

TArray<int32> UGSS_GameSettingValueDiscreteDynamic::GetEnabledOptionIndices() const
{
	const TArray<FString>& DisabledOptions = GetEditState().GetDisabledOptions();
	TArray<int32> EnabledIndices;
	EnabledIndices.Reserve(OptionValues.Num());
	for (int32 OptionIndex = 0; OptionIndex < OptionValues.Num(); ++OptionIndex)
	{
		const bool bDisabled = DisabledOptions.ContainsByPredicate([this, OptionIndex](const FString& DisabledOption)
		{
			return AreOptionsEqual(DisabledOption, OptionValues[OptionIndex]);
		});
		if (!bDisabled)
		{
			EnabledIndices.Add(OptionIndex);
		}
	}
	return EnabledIndices;
}

int32 UGSS_GameSettingValueDiscreteDynamic::GetEnabledOptionIndex(int32 RawOptionIndex) const
{
	return GetEnabledOptionIndices().IndexOfByKey(RawOptionIndex);
}

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Bool
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic_Bool::UGSS_GameSettingValueDiscreteDynamic_Bool()
{
	AddDynamicOption(TEXT("false"), LOCTEXT("OFF", "OFF"));
	AddDynamicOption(TEXT("true"), LOCTEXT("ON", "ON"));
}

void UGSS_GameSettingValueDiscreteDynamic_Bool::SetTrueText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("true"));
	AddDynamicOption(TEXT("true"), InText);
}

void UGSS_GameSettingValueDiscreteDynamic_Bool::SetFalseText(const FText& InText)
{
	// We remove and then re-add it, so that by changing the true/false text you can also control the order they appear.
	RemoveDynamicOption(TEXT("false"));
	AddDynamicOption(TEXT("false"), InText);
}

void UGSS_GameSettingValueDiscreteDynamic_Bool::SetDefaultValue(bool Value)
{
	DefaultValue = LexToString(Value);
}

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Number
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic_Number::UGSS_GameSettingValueDiscreteDynamic_Number()
{

}

void UGSS_GameSettingValueDiscreteDynamic_Number::OnInitialized()
{
	Super::OnInitialized();

	ensure(OptionValues.Num() > 0);
}

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Enum
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic_Enum::UGSS_GameSettingValueDiscreteDynamic_Enum()
{

}

void UGSS_GameSettingValueDiscreteDynamic_Enum::OnInitialized()
{
	Super::OnInitialized();

	ensure(OptionValues.Num() > 0);
}

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Color
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic_Color::UGSS_GameSettingValueDiscreteDynamic_Color()
{

}


#undef LOCTEXT_NAMESPACE
