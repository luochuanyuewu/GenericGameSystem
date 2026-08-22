// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Language.h"

#include "Settings/GSS_GameSettingFilterState.h"
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
	static constexpr int32 SystemDefaultIndex = 0;
}

void UGSS_GameSettingValueDiscrete_Language::OnInitialized()
{
	RefreshCultureOptions();
	Super::OnInitialized();
}

void UGSS_GameSettingValueDiscrete_Language::RefreshCultureOptions()
{
	AvailableCultureNames.Reset();
	AvailableCultureNames.Add(FString());

	const TArray<FString> AllCultureNames = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (const FString& CultureName : AllCultureNames)
	{
		if (FInternationalization::Get().IsCultureAllowed(CultureName) && !AvailableCultureNames.Contains(CultureName))
		{
			AvailableCultureNames.Add(CultureName);
		}
	}
}

void UGSS_GameSettingValueDiscrete_Language::StoreInitial()
{
	InitialValue = IsUsingDefaultCulture() ? FString() : GetCurrentCultureName();
	if (const int32 MatchingIndex = FindBestOptionIndex(InitialValue); MatchingIndex != INDEX_NONE)
	{
		InitialValue = AvailableCultureNames[MatchingIndex];
	}
	PendingCulture = InitialValue;
}

void UGSS_GameSettingValueDiscrete_Language::ResetToDefault()
{
	SetDiscreteOptionByIndex(GSS_Language::SystemDefaultIndex);
}

void UGSS_GameSettingValueDiscrete_Language::RestoreToInitial()
{
	PendingCulture = InitialValue;
	NotifySettingChanged(EGSS_GameSettingChangeReason::RestoreToInitial);
}

void UGSS_GameSettingValueDiscrete_Language::SetDiscreteOptionByIndex(int32 Index)
{
	if (AvailableCultureNames.IsValidIndex(Index))
	{
		PendingCulture = AvailableCultureNames[Index];
		NotifySettingChanged(EGSS_GameSettingChangeReason::Change);
	}
}

int32 UGSS_GameSettingValueDiscrete_Language::GetDiscreteOptionIndex() const
{
	const int32 MatchingIndex = FindBestOptionIndex(PendingCulture);
	return MatchingIndex != INDEX_NONE ? MatchingIndex : GetDiscreteOptionDefaultIndex();
}

int32 UGSS_GameSettingValueDiscrete_Language::GetDiscreteOptionDefaultIndex() const
{
	return GSS_Language::SystemDefaultIndex;
}

TArray<FText> UGSS_GameSettingValueDiscrete_Language::GetDiscreteOptions() const
{
	TArray<FText> Options;
	Options.Reserve(AvailableCultureNames.Num());
	for (const FString& CultureName : AvailableCultureNames)
	{
		Options.Add(BuildCultureDisplayName(CultureName));
	}
	return Options;
}

void UGSS_GameSettingValueDiscrete_Language::OnApply()
{
	if (!GConfig)
	{
		return;
	}

	if (PendingCulture.IsEmpty())
	{
		const FCulturePtr SystemDefaultCulture = FInternationalization::Get().GetDefaultCulture();
		if (!SystemDefaultCulture.IsValid() || !FInternationalization::Get().SetCurrentCulture(SystemDefaultCulture->GetName()))
		{
			return;
		}

		GConfig->RemoveKey(GSS_Language::ConfigSection, GSS_Language::ConfigKey, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);
		return;
	}

	if (!FInternationalization::Get().SetCurrentCulture(PendingCulture))
	{
		return;
	}

	GConfig->SetString(GSS_Language::ConfigSection, GSS_Language::ConfigKey, *PendingCulture, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
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
	const int32 ExactMatchIndex = AvailableCultureNames.IndexOfByKey(CultureName);
	if (ExactMatchIndex != INDEX_NONE)
	{
		return ExactMatchIndex;
	}

	if (CultureName.IsEmpty())
	{
		return INDEX_NONE;
	}

	const TArray<FString> PrioritizedCultures = FInternationalization::Get().GetPrioritizedCultureNames(CultureName);
	for (int32 OptionIndex = 0; OptionIndex < AvailableCultureNames.Num(); ++OptionIndex)
	{
		if (!AvailableCultureNames[OptionIndex].IsEmpty() && PrioritizedCultures.Contains(AvailableCultureNames[OptionIndex]))
		{
			return OptionIndex;
		}
	}

	return INDEX_NONE;
}

#undef LOCTEXT_NAMESPACE
