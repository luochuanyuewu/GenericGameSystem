// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/DataSource/GSS_GameSettingDataSource.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscreteDynamic)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscreteDynamic"

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueDiscreteDynamic::UGSS_GameSettingValueDiscreteDynamic()
{
}

void UGSS_GameSettingValueDiscreteDynamic::SetDynamicGetter(const TSharedRef<FGSS_GameSettingDataSource>& InGetter)
{
	Getter = InGetter;
}

void UGSS_GameSettingValueDiscreteDynamic::SetDynamicSetter(const TSharedRef<FGSS_GameSettingDataSource>& InSetter)
{
	Setter = InSetter;
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
	return OptionValues.Contains(InOptionValue);
}

FString UGSS_GameSettingValueDiscreteDynamic::GetValueAsString() const
{
	return Getter->GetValueAsString(LocalPlayer);
}

void UGSS_GameSettingValueDiscreteDynamic::SetValueFromString(FString InStringValue)
{
	SetValueFromString(InStringValue, EGSS_GameSettingChangeReason::Change);
}

void UGSS_GameSettingValueDiscreteDynamic::SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason)
{
	check(Setter);
	Setter->SetValue(LocalPlayer, InStringValue);

	NotifySettingChanged(Reason);
}

bool UGSS_GameSettingValueDiscreteDynamic::AreOptionsEqual(const FString& InOptionA, const FString& InOptionB) const
{
	return InOptionA == InOptionB;
}

void UGSS_GameSettingValueDiscreteDynamic::OnInitialized()
{
#if !UE_BUILD_SHIPPING
	ensureAlways(Getter);
	ensureAlwaysMsgf(Getter->Resolve(LocalPlayer), TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the getter function have no parameters?"), *GetDevName().ToString(), *Getter->ToString());
	ensureAlways(Setter);
	ensureAlwaysMsgf(Setter->Resolve(LocalPlayer), TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties? Does the setting function have exactly one parameter?"), *GetDevName().ToString(), *Setter->ToString());
#endif

	Super::OnInitialized();
}

void UGSS_GameSettingValueDiscreteDynamic::Startup()
{
	// Should I also do something with Setter?
	check(Getter);
	Getter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnDataSourcesReady));
}

void UGSS_GameSettingValueDiscreteDynamic::OnDataSourcesReady()
{
	StartupComplete();
}

void UGSS_GameSettingValueDiscreteDynamic::StoreInitial()
{
	InitialValue = GetValueAsString();
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

void UGSS_GameSettingValueDiscreteDynamic::SetDiscreteOptionByIndex(int32 Index)
{
	if (ensure(OptionValues.IsValidIndex(Index)))
	{
		SetValueFromString(OptionValues[Index]);
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

	return Index;
}

int32 UGSS_GameSettingValueDiscreteDynamic::GetDiscreteOptionDefaultIndex() const
{
	if (DefaultValue.IsSet())
	{
		return OptionValues.IndexOfByPredicate([this](const FString& InOption) {
			return AreOptionsEqual(DefaultValue.GetValue(), InOption);
		});
	}

	return INDEX_NONE;
}

TArray<FText> UGSS_GameSettingValueDiscreteDynamic::GetDiscreteOptions() const
{
	const TArray<FString>& DisabledOptions = GetEditState().GetDisabledOptions();

	if (DisabledOptions.Num() > 0)
	{
		TArray<FText> AllowedOptions;

		for (int32 OptionIndex = 0; OptionIndex < OptionValues.Num(); ++OptionIndex)
		{
			if (!DisabledOptions.Contains(OptionValues[OptionIndex]))
			{
				AllowedOptions.Add(OptionDisplayTexts[OptionIndex]);
			}
		}

		return AllowedOptions;
	}

	return OptionDisplayTexts;
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
