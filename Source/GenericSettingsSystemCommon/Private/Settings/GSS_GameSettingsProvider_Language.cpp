// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider_Language.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingValueDiscrete_Language.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider_Language)

#define LOCTEXT_NAMESPACE "GSS_GameSettingsProvider_Language"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Language, "GSS.Settings.Language");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Language_Culture, "GSS.Settings.Language.Culture");

UGSS_GameSettingsProvider_Language::UGSS_GameSettingsProvider_Language()
{
	LanguageText = LOCTEXT("Language", "Language");
	LanguageDescriptionText = LOCTEXT("LanguageDescription", "Choose the game display language.");
	CultureText = LOCTEXT("Culture", "Language");
	CultureDescriptionText = LOCTEXT("CultureDescription", "Choose the language used by the game.");
	CultureRestartWarningText = LOCTEXT("CultureRestartWarning", "You may need to restart the game for all language-related changes to take effect.");
}

void UGSS_GameSettingsProvider_Language::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* LanguageCollection = Builder->AddCollection(TAG_GSS_Settings_Language, LanguageText, LanguageDescriptionText);
	UGSS_GameSettingValueDiscrete_Language* CultureSetting = NewObject<UGSS_GameSettingValueDiscrete_Language>(Builder);
	CultureSetting->SetSettingId(TAG_GSS_Settings_Language_Culture);
	CultureSetting->SetDevName(FName(TEXT("GameLanguage")));
	CultureSetting->SetDisplayName(CultureText);
	CultureSetting->SetDescriptionRichText(CultureDescriptionText);
	CultureSetting->SetWarningRichText(CultureRestartWarningText);
	Builder->AddRuntimeSetting(CultureSetting, LanguageCollection);
}

#undef LOCTEXT_NAMESPACE
