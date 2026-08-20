// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscrete_Language.h"
#include "Settings/GSS_LanguageSettingsProvider.h"
#include "Settings/GSS_GameSettingsProvider.h"
#include "Settings/GSS_VideoSettingsProvider.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_CommonVideoInclusionTest, "GSS.Settings.Common.VideoInclusion", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_CommonVideoInclusionTest::RunTest(const FString& Parameters)
{
	UGSS_GameSettingRegistry* Registry = NewObject<UGSS_GameSettingRegistry>();
	UGSS_GameSettingsBuilder* Builder = NewObject<UGSS_GameSettingsBuilder>();
	Builder->Initialize(nullptr, Registry);

	UGSS_VideoSettingsProvider* Provider = NewObject<UGSS_VideoSettingsProvider>();
	Provider->IncludedSettings.bWindowMode = false;
	Provider->IncludedSettings.bVSync = false;
	Provider->IncludedSettings.bIndividualQuality = false;
	Provider->RegisterSettings_Implementation(Builder);

	const FGameplayTag WindowModeId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.WindowMode"), false);
	const FGameplayTag ResolutionId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.Display.Resolution"), false);
	const FGameplayTag BrightnessId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.Display.Brightness"), false);
	const FGameplayTag VSyncId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.Advanced.VSync"), false);
	const FGameplayTag OverallQualityId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.Quality.Overall"), false);
	const FGameplayTag ViewDistanceId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Video.Quality.ViewDistance"), false);

	TestNull(TEXT("Unchecked Window Mode is not registered"), Registry->FindSettingById(WindowModeId));
	TestNull(TEXT("Unchecked VSync is not registered"), Registry->FindSettingById(VSyncId));
	TestNull(TEXT("Unchecked individual quality is not registered"), Registry->FindSettingById(ViewDistanceId));
	TestNotNull(TEXT("Resolution remains registered when Window Mode is omitted"), Registry->FindSettingById(ResolutionId));
	TestNotNull(TEXT("Brightness remains registered by default"), Registry->FindSettingById(BrightnessId));
	TestNotNull(TEXT("Overall quality remains registered when individual quality is omitted"), Registry->FindSettingById(OverallQualityId));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_CommonLanguageProviderTest, "GSS.Settings.Common.LanguageProvider", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_CommonLanguageProviderTest::RunTest(const FString& Parameters)
{
	UGSS_GameSettingRegistry* Registry = NewObject<UGSS_GameSettingRegistry>();
	UGSS_GameSettingsBuilder* Builder = NewObject<UGSS_GameSettingsBuilder>();
	Builder->Initialize(nullptr, Registry);

	UGSS_LanguageSettingsProvider* Provider = NewObject<UGSS_LanguageSettingsProvider>();
	Provider->RegisterSettings_Implementation(Builder);

	const FGameplayTag CultureId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Language.Culture"), false);
	UGSS_GameSettingValueDiscrete_Language* Language = Cast<UGSS_GameSettingValueDiscrete_Language>(Registry->FindSettingById(CultureId));
	TestNotNull(TEXT("Language provider registers a culture setting"), Language);
	if (!Language)
	{
		return true;
	}

	Language->RefreshCultureOptions();
	TestTrue(TEXT("Language options include System Default"), Language->HasDynamicOption(TEXT("")));
	TestTrue(TEXT("Language exposes at least the System Default option"), Language->GetDiscreteOptions().Num() >= 1);
	return true;
}

#endif
