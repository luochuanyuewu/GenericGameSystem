// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_LanguageSettingsProvider.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingValueDiscrete_Language.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_LanguageSettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_LanguageSettingsProvider"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Language, "GSS.Settings.Language");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Language_Culture, "GSS.Settings.Language.Culture");

void UGSS_LanguageSettingsProvider::RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Language = Builder->AddCollection(TAG_GSS_Settings_Language, LOCTEXT("Language", "Language"), LOCTEXT("LanguageDescription", "Choose the game display language."));
	UGSS_GameSettingValueDiscrete_Language* Culture = NewObject<UGSS_GameSettingValueDiscrete_Language>(Builder);
	Culture->SetSettingId(TAG_GSS_Settings_Language_Culture);
	Culture->SetDevName(FName(TEXT("GameLanguage")));
	Culture->SetDisplayName(LOCTEXT("Culture", "Language"));
	Culture->SetDescriptionRichText(LOCTEXT("CultureDescription", "Choose the language used by the game."));
	Culture->SetWarningRichText(LOCTEXT("CultureRestartWarning", "You may need to restart the game for all language-related changes to take effect."));
	Builder->AddRuntimeSetting(Culture, Language);
}

#undef LOCTEXT_NAMESPACE
