// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_AccessibilitySettingsProvider.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_CommonSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_AccessibilitySettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_AccessibilitySettingsProvider"

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

FGSS_DiscreteOptionDefinition UGSS_AccessibilitySettingsProvider::MakeOption(const TCHAR* Value, const FText& DisplayName)
{
	FGSS_DiscreteOptionDefinition Option;
	Option.Value = Value;
	Option.DisplayName = DisplayName;
	return Option;
}

void UGSS_AccessibilitySettingsProvider::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Accessibility, LOCTEXT("Accessibility", "Accessibility"), LOCTEXT("AccessibilityDescription", "Configure accessibility preferences."));
	UGSS_GameSetting* Subtitles = Builder->AddCollection(TAG_GSS_Settings_Accessibility_Subtitles, LOCTEXT("Subtitles", "Subtitles"), FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Subtitles_Enabled, LOCTEXT("SubtitlesEnabled", "Display Subtitles"), LOCTEXT("SubtitlesEnabledDescription", "Display dialogue subtitles when available."), true, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitlesEnabled, SetSubtitlesEnabled), Subtitles);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Subtitles_TextScale, LOCTEXT("SubtitleTextScale", "Text Scale"), LOCTEXT("SubtitleTextScaleDescription", "Adjust subtitle text size."), 1.0, 0.5, 2.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleTextScale, SetSubtitleTextScale), Subtitles);
	Builder->AddDiscrete(TAG_GSS_Settings_Accessibility_Subtitles_TextColor, LOCTEXT("SubtitleTextColor", "Text Color"), LOCTEXT("SubtitleTextColorDescription", "Choose the subtitle text color."), TEXT("White"),
		{
			MakeOption(TEXT("White"), LOCTEXT("SubtitleColorWhite", "White")),
			MakeOption(TEXT("Yellow"), LOCTEXT("SubtitleColorYellow", "Yellow")),
			MakeOption(TEXT("Cyan"), LOCTEXT("SubtitleColorCyan", "Cyan")),
		}, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleTextColor, SetSubtitleTextColor), Subtitles);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Subtitles_Border, LOCTEXT("SubtitleBorder", "Text Border"), LOCTEXT("SubtitleBorderDescription", "Draw a border around subtitle text."), true, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleBorderEnabled, SetSubtitleBorderEnabled), Subtitles);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Subtitles_BackgroundOpacity, LOCTEXT("SubtitleBackgroundOpacity", "Background Opacity"), LOCTEXT("SubtitleBackgroundOpacityDescription", "Adjust subtitle background opacity."), 0.5, 0.0, 1.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSubtitleBackgroundOpacity, SetSubtitleBackgroundOpacity), Subtitles);

	UGSS_GameSetting* ColorVision = Builder->AddCollection(TAG_GSS_Settings_Accessibility_ColorVision, LOCTEXT("ColorVision", "Color Vision"), FText::GetEmpty(), Screen);
	Builder->AddDiscrete(TAG_GSS_Settings_Accessibility_ColorVision_Mode, LOCTEXT("ColorBlindMode", "Color Vision Deficiency Mode"), LOCTEXT("ColorBlindModeDescription", "Choose a color vision correction mode."), TEXT("None"),
		{
			MakeOption(TEXT("None"), LOCTEXT("ColorBlindNone", "None")),
			MakeOption(TEXT("Deuteranope"), LOCTEXT("ColorBlindDeuteranope", "Deuteranope")),
			MakeOption(TEXT("Protanope"), LOCTEXT("ColorBlindProtanope", "Protanope")),
			MakeOption(TEXT("Tritanope"), LOCTEXT("ColorBlindTritanope", "Tritanope")),
		}, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetColorVisionMode, SetColorVisionMode), ColorVision);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_ColorVision_Strength, LOCTEXT("ColorBlindStrength", "Color Correction Strength"), LOCTEXT("ColorBlindStrengthDescription", "Control the strength of color correction."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetColorVisionStrength, SetColorVisionStrength), ColorVision);
	UGSS_GameSetting* Motion = Builder->AddCollection(TAG_GSS_Settings_Accessibility_Motion, LOCTEXT("Motion", "Motion"), FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Accessibility_Motion_ReduceCameraMotion, LOCTEXT("ReduceCameraMotion", "Reduce Camera Motion"), LOCTEXT("ReduceCameraMotionDescription", "Request reduced camera motion when supported."), false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetReduceCameraMotion, SetReduceCameraMotion), Motion);
	Builder->AddScalar(TAG_GSS_Settings_Accessibility_Motion_ScreenShakeIntensity, LOCTEXT("ScreenShakeIntensity", "Screen Shake Intensity"), LOCTEXT("ScreenShakeIntensityDescription", "Control the preferred screen shake intensity."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetScreenShakeIntensity, SetScreenShakeIntensity), Motion);
}

#undef LOCTEXT_NAMESPACE
