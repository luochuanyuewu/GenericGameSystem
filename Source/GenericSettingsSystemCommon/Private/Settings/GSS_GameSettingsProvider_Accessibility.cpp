// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider_Accessibility.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_CommonSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider_Accessibility)

#define LOCTEXT_NAMESPACE "GSS_GameSettingsProvider_Accessibility"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility, "GSS.Settings.Accessibility");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles, "GSS.Settings.Accessibility.Subtitles");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles_Enabled, "GSS.Settings.Accessibility.Subtitles.Enabled");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles_TextScale, "GSS.Settings.Accessibility.Subtitles.TextScale");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles_TextColor, "GSS.Settings.Accessibility.Subtitles.TextColor");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles_Border, "GSS.Settings.Accessibility.Subtitles.Border");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Subtitles_BackgroundOpacity, "GSS.Settings.Accessibility.Subtitles.BackgroundOpacity");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_ColorVision, "GSS.Settings.Accessibility.ColorVision");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_ColorVision_Mode, "GSS.Settings.Accessibility.ColorVision.Mode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_ColorVision_Strength, "GSS.Settings.Accessibility.ColorVision.Strength");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Motion, "GSS.Settings.Accessibility.Motion");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Motion_ReduceCameraMotion, "GSS.Settings.Accessibility.Motion.ReduceCameraMotion");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Accessibility_Motion_ScreenShakeIntensity, "GSS.Settings.Accessibility.Motion.ScreenShakeIntensity");

UGSS_GameSettingsProvider_Accessibility::UGSS_GameSettingsProvider_Accessibility()
{
	AccessibilityText = LOCTEXT("Accessibility", "Accessibility");
	AccessibilityDescriptionText = LOCTEXT("AccessibilityDescription", "Configure accessibility preferences.");
	SubtitlesText = LOCTEXT("Subtitles", "Subtitles");
	SubtitlesEnabledText = LOCTEXT("SubtitlesEnabled", "Display Subtitles");
	SubtitlesEnabledDescriptionText = LOCTEXT("SubtitlesEnabledDescription", "Display dialogue subtitles when available.");
	SubtitleTextScaleText = LOCTEXT("SubtitleTextScale", "Text Scale");
	SubtitleTextScaleDescriptionText = LOCTEXT("SubtitleTextScaleDescription", "Adjust subtitle text size.");
	SubtitleTextColorText = LOCTEXT("SubtitleTextColor", "Text Color");
	SubtitleTextColorDescriptionText = LOCTEXT("SubtitleTextColorDescription", "Choose the subtitle text color.");
	SubtitleColorWhiteText = LOCTEXT("SubtitleColorWhite", "White");
	SubtitleColorYellowText = LOCTEXT("SubtitleColorYellow", "Yellow");
	SubtitleColorCyanText = LOCTEXT("SubtitleColorCyan", "Cyan");
	SubtitleBorderText = LOCTEXT("SubtitleBorder", "Text Border");
	SubtitleBorderDescriptionText = LOCTEXT("SubtitleBorderDescription", "Draw a border around subtitle text.");
	SubtitleBackgroundOpacityText = LOCTEXT("SubtitleBackgroundOpacity", "Background Opacity");
	SubtitleBackgroundOpacityDescriptionText = LOCTEXT("SubtitleBackgroundOpacityDescription", "Adjust subtitle background opacity.");
	ColorVisionText = LOCTEXT("ColorVision", "Color Vision");
	ColorBlindModeText = LOCTEXT("ColorBlindMode", "Color Vision Deficiency Mode");
	ColorBlindModeDescriptionText = LOCTEXT("ColorBlindModeDescription", "Choose a color vision correction mode.");
	ColorBlindNoneText = LOCTEXT("ColorBlindNone", "None");
	ColorBlindDeuteranopeText = LOCTEXT("ColorBlindDeuteranope", "Deuteranope");
	ColorBlindProtanopeText = LOCTEXT("ColorBlindProtanope", "Protanope");
	ColorBlindTritanopeText = LOCTEXT("ColorBlindTritanope", "Tritanope");
	ColorBlindStrengthText = LOCTEXT("ColorBlindStrength", "Color Correction Strength");
	ColorBlindStrengthDescriptionText = LOCTEXT("ColorBlindStrengthDescription", "Control the strength of color correction.");
	MotionText = LOCTEXT("Motion", "Motion");
	ReduceCameraMotionText = LOCTEXT("ReduceCameraMotion", "Reduce Camera Motion");
	ReduceCameraMotionDescriptionText = LOCTEXT("ReduceCameraMotionDescription", "Request reduced camera motion when supported.");
	ScreenShakeIntensityText = LOCTEXT("ScreenShakeIntensity", "Screen Shake Intensity");
	ScreenShakeIntensityDescriptionText = LOCTEXT("ScreenShakeIntensityDescription", "Control the preferred screen shake intensity.");
}

FGSS_DiscreteOptionDefinition UGSS_GameSettingsProvider_Accessibility::MakeOption(const TCHAR* Value, const FText& DisplayName)
{
	FGSS_DiscreteOptionDefinition Option;
	Option.Value = Value;
	Option.DisplayName = DisplayName;
	return Option;
}

void UGSS_GameSettingsProvider_Accessibility::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Accessibility, AccessibilityText, AccessibilityDescriptionText);
	UGSS_GameSetting* SubtitlesCollection = Builder->AddCollection(TAG_GSS_Settings_Accessibility_Subtitles, SubtitlesText, FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Subtitles_Enabled, SubtitlesEnabledText, SubtitlesEnabledDescriptionText, true, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitlesEnabled, SetSubtitlesEnabled), SubtitlesCollection);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Subtitles_TextScale, SubtitleTextScaleText, SubtitleTextScaleDescriptionText, 1.0, 0.5, 2.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleTextScale, SetSubtitleTextScale), SubtitlesCollection);
	Builder->AddDiscrete(TAG_GSS_Settings_Accessibility_Subtitles_TextColor, SubtitleTextColorText, SubtitleTextColorDescriptionText, TEXT("White"),
		{
			MakeOption(TEXT("White"), SubtitleColorWhiteText),
			MakeOption(TEXT("Yellow"), SubtitleColorYellowText),
			MakeOption(TEXT("Cyan"), SubtitleColorCyanText),
	}, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleTextColor, SetSubtitleTextColor), SubtitlesCollection);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Subtitles_Border, SubtitleBorderText, SubtitleBorderDescriptionText, true, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleBorderEnabled, SetSubtitleBorderEnabled), SubtitlesCollection);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Subtitles_BackgroundOpacity, SubtitleBackgroundOpacityText, SubtitleBackgroundOpacityDescriptionText, 0.5, 0.0, 1.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleBackgroundOpacity, SetSubtitleBackgroundOpacity), SubtitlesCollection);

	UGSS_GameSetting* ColorVisionCollection = Builder->AddCollection(TAG_GSS_Settings_Accessibility_ColorVision, ColorVisionText, FText::GetEmpty(), Screen);
	Builder->AddDiscrete(TAG_GSS_Settings_Accessibility_ColorVision_Mode, ColorBlindModeText, ColorBlindModeDescriptionText, TEXT("None"),
		{
			MakeOption(TEXT("None"), ColorBlindNoneText),
			MakeOption(TEXT("Deuteranope"), ColorBlindDeuteranopeText),
			MakeOption(TEXT("Protanope"), ColorBlindProtanopeText),
			MakeOption(TEXT("Tritanope"), ColorBlindTritanopeText),
	}, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetColorVisionMode, SetColorVisionMode), ColorVisionCollection);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_ColorVision_Strength, ColorBlindStrengthText, ColorBlindStrengthDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetColorVisionStrength, SetColorVisionStrength), ColorVisionCollection);
	UGSS_GameSetting* MotionCollection = Builder->AddCollection(TAG_GSS_Settings_Accessibility_Motion, MotionText, FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Motion_ReduceCameraMotion, ReduceCameraMotionText, ReduceCameraMotionDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetReduceCameraMotion, SetReduceCameraMotion), MotionCollection);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Motion_ScreenShakeIntensity, ScreenShakeIntensityText, ScreenShakeIntensityDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetScreenShakeIntensity, SetScreenShakeIntensity), MotionCollection);
}

#undef LOCTEXT_NAMESPACE
