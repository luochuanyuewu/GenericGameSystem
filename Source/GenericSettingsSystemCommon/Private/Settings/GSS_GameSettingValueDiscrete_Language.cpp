// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Language.h"

#include "Settings/GSS_GameSettingValue.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/TextLocalizationManager.h"
#include "Misc/ConfigCacheIni.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Language)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscrete_Language"

namespace GSS_Language
{
	static const TCHAR* ConfigSection = TEXT("Internationalization");
	static const TCHAR* ConfigKey = TEXT("Culture");
	static const TCHAR* SystemDefaultValue = TEXT("");
}

void UGSS_GameSettingValueDiscrete_Language::OnInitialized()
{
	RefreshCultureOptions();
	SetDefaultValueFromString(GSS_Language::SystemDefaultValue);
	UGSS_GameSettingValue::OnInitialized();
}

void UGSS_GameSettingValueDiscrete_Language::RefreshCultureOptions()
{
	const TArray<FString> ExistingOptions = GetDynamicOptions();
	for (const FString& Option : ExistingOptions)
	{
		RemoveDynamicOption(Option);
	}

	AddDynamicOption(GSS_Language::SystemDefaultValue, BuildCultureDisplayName(GSS_Language::SystemDefaultValue));

	const TArray<FString> AllCultureNames = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (const FString& CultureName : AllCultureNames)
	{
		if (FInternationalization::Get().IsCultureAllowed(CultureName) && !HasDynamicOption(CultureName))
		{
			AddDynamicOption(CultureName, BuildCultureDisplayName(CultureName));
		}
	}
}

void UGSS_GameSettingValueDiscrete_Language::StoreInitial()
{
	InitialValue = IsUsingDefaultCulture() ? FString(GSS_Language::SystemDefaultValue) : GetCurrentCultureName();
	if (const int32 MatchingIndex = FindBestOptionIndex(InitialValue); MatchingIndex != INDEX_NONE)
	{
		InitialValue = OptionValues[MatchingIndex];
	}
	PendingValue = InitialValue;
}

int32 UGSS_GameSettingValueDiscrete_Language::GetDiscreteOptionIndex() const
{
	const int32 MatchingIndex = FindBestOptionIndex(GetValueAsString());
	if (MatchingIndex != INDEX_NONE)
	{
		return GetEnabledOptionIndex(MatchingIndex);
	}
	return GetDiscreteOptionDefaultIndex();
}

bool UGSS_GameSettingValueDiscrete_Language::OnApply()
{
	if (!GConfig)
	{
		return false;
	}

	if (PendingValue.IsEmpty())
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		if (!SystemDefaultCulture.IsValid())
		{
			return false;
		}

		if (!FInternationalization::Get().SetCurrentCulture(SystemDefaultCulture->GetName()))
		{
			return false;
		}

		GConfig->RemoveKey(GSS_Language::ConfigSection, GSS_Language::ConfigKey, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);
		return true;
	}

	if (!FInternationalization::Get().SetCurrentCulture(PendingValue))
	{
		return false;
	}

	GConfig->SetString(GSS_Language::ConfigSection, GSS_Language::ConfigKey, *PendingValue, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
	return true;
}

bool UGSS_GameSettingValueDiscrete_Language::IsUsingDefaultCulture()
{
	FString Culture;
	return !GConfig || !GConfig->GetString(GSS_Language::ConfigSection, GSS_Language::ConfigKey, Culture, GGameUserSettingsIni) || Culture.IsEmpty();
}

FString UGSS_GameSettingValueDiscrete_Language::GetCurrentCultureName()
{
	const FCulturePtr CurrentCulture = FInternationalization::Get().GetCurrentCulture();
	return CurrentCulture.IsValid() ? CurrentCulture->GetName() : FString();
}

FText UGSS_GameSettingValueDiscrete_Language::BuildCultureDisplayName(const FString& CultureName)
{
	if (CultureName.IsEmpty())
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		if (SystemDefaultCulture.IsValid())
		{
			return FText::Format(LOCTEXT("SystemDefaultLanguage", "System Default ({0})"), FText::FromString(SystemDefaultCulture->GetDisplayName()));
		}
		return LOCTEXT("SystemDefaultLanguageFallback", "System Default");
	}

	const FCulturePtr Culture = FInternationalization::Get().GetCulture(CultureName);
	if (!Culture.IsValid())
	{
		return FText::FromString(CultureName);
	}

	const FString NativeName = Culture->GetNativeName();
	const FString DisplayName = Culture->GetDisplayName();
	return NativeName.Equals(DisplayName, ESearchCase::CaseSensitive)
		? FText::FromString(NativeName)
		: FText::FromString(FString::Printf(TEXT("%s (%s)"), *NativeName, *DisplayName));
}

int32 UGSS_GameSettingValueDiscrete_Language::FindBestOptionIndex(const FString& CultureName) const
{
	const int32 ExactMatchIndex = OptionValues.IndexOfByPredicate([this, &CultureName](const FString& Option)
	{
		return AreOptionsEqual(CultureName, Option);
	});
	if (ExactMatchIndex != INDEX_NONE)
	{
		return ExactMatchIndex;
	}

	if (CultureName.IsEmpty())
	{
		return INDEX_NONE;
	}

	const TArray<FString> PrioritizedCultures = FInternationalization::Get().GetPrioritizedCultureNames(CultureName);
	for (int32 OptionIndex = 0; OptionIndex < OptionValues.Num(); ++OptionIndex)
	{
		if (!OptionValues[OptionIndex].IsEmpty() && PrioritizedCultures.Contains(OptionValues[OptionIndex]))
		{
			return OptionIndex;
		}
	}

	return INDEX_NONE;
}

#undef LOCTEXT_NAMESPACE
