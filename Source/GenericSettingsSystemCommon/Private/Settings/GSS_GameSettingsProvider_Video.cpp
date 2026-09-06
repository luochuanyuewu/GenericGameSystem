// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider_Video.h"

#include "NativeGameplayTags.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/EditCondition/GSS_WhenPlatformHasTrait.h"
#include "Settings/EditCondition/GSS_WhenPlatformSupportsWindowedMode.h"
#include "Settings/EditCondition/GSS_WhenPlayingAsPrimaryPlayer.h"
#include "Settings/EditCondition/GSS_WhenSettingHasValue.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingValueDiscrete_Display.h"
#include "Settings/GSS_GameSettingValueDiscrete_OverallQuality.h"
#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"
#include "Settings/GSS_GameSettingValueScalar_Brightness.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider_Video)

#define LOCTEXT_NAMESPACE "GSS_GameSettingsProvider_Video"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video, "GSS.Settings.Video");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Display, "GSS.Settings.Video.Display");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_WindowMode, "GSS.Settings.Video.WindowMode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Display_Monitor, "GSS.Settings.Video.Display.Monitor");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Resolution, "GSS.Settings.Video.Display.Resolution");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Brightness, "GSS.Settings.Video.Display.Brightness");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_SupportsWindowedMode, "Platform.Trait.SupportsWindowedMode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_NeedsBrightnessAdjustment, "Platform.Trait.NeedsBrightnessAdjustment");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Platform_Trait_SupportsCustomDynamicResolution, "Platform.Trait.SupportsCustomDynamicResolution");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality, "GSS.Settings.Video.Quality");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Overall, "GSS.Settings.Video.Quality.Overall");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Advanced, "GSS.Settings.Video.Quality.Advanced");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_ViewDistance, "GSS.Settings.Video.Quality.ViewDistance");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_AntiAliasing, "GSS.Settings.Video.Quality.AntiAliasing");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Shadows, "GSS.Settings.Video.Quality.Shadows");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_GlobalIllumination, "GSS.Settings.Video.Quality.GlobalIllumination");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Reflections, "GSS.Settings.Video.Quality.Reflections");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_PostProcessing, "GSS.Settings.Video.Quality.PostProcessing");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Textures, "GSS.Settings.Video.Quality.Textures");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_VisualEffects, "GSS.Settings.Video.Quality.VisualEffects");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Foliage, "GSS.Settings.Video.Quality.Foliage");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Quality_Shading, "GSS.Settings.Video.Quality.Shading");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Advanced, "GSS.Settings.Video.Advanced");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Advanced_VSync, "GSS.Settings.Video.Advanced.VSync");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Advanced_FrameRateLimit, "GSS.Settings.Video.Advanced.FrameRateLimit");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Advanced_DynamicResolution, "GSS.Settings.Video.Advanced.DynamicResolution");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Advanced_ResolutionScale, "GSS.Settings.Video.Advanced.ResolutionScale");

UGSS_GameSettingsProvider_Video::UGSS_GameSettingsProvider_Video()
{
	QualityLowText = LOCTEXT("QualityLow", "Low");
	QualityMediumText = LOCTEXT("QualityMedium", "Medium");
	QualityHighText = LOCTEXT("QualityHigh", "High");
	QualityEpicText = LOCTEXT("QualityEpic", "Epic");
	QualityCinematicText = LOCTEXT("QualityCinematic", "Cinematic");
	VideoText = LOCTEXT("Video", "Video");
	VideoDescriptionText = LOCTEXT("VideoDescription", "Configure display and graphics preferences.");
	DisplayText = LOCTEXT("Display", "Display");
	DisplayMonitorText = LOCTEXT("DisplayMonitor", "Display");
	DisplayMonitorDescriptionText = LOCTEXT("DisplayMonitorDescription", "The display on which the window should be shown.");
	WindowModeText = LOCTEXT("WindowMode", "Window Mode");
	WindowModeDescriptionText = LOCTEXT("WindowModeDescription", "Choose how the game window is displayed.");
	FullscreenText = LOCTEXT("Fullscreen", "Fullscreen");
	WindowedFullscreenText = LOCTEXT("WindowedFullscreen", "Windowed Fullscreen");
	WindowedText = LOCTEXT("Windowed", "Windowed");
	ResolutionText = LOCTEXT("Resolution", "Resolution");
	ResolutionDescriptionText = LOCTEXT("ResolutionDescription", "Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode.");
	ResolutionDisabledWindowedFullscreenText = LOCTEXT("ResolutionDisabledWindowedFullscreen", "Resolution is controlled by the desktop in Windowed Fullscreen.");
	BrightnessText = LOCTEXT("Brightness", "Brightness");
	BrightnessDescriptionText = LOCTEXT("BrightnessDescription", "Adjust display brightness.");
	QualityText = LOCTEXT("Quality", "Graphics Quality");
	OverallQualityText = LOCTEXT("OverallQuality", "Quality Preset");
	OverallQualityDescriptionText = LOCTEXT("OverallQualityDescription", "Apply one graphics quality level to all scalability groups.");
	QualityAdvancedText = LOCTEXT("QualityAdvanced", "Individual Quality");
	QualityAdvancedDescriptionText = LOCTEXT("QualityAdvancedDescription", "Fine-tune individual graphics features.");
	ViewDistanceText = LOCTEXT("ViewDistance", "View Distance");
	ViewDistanceDescriptionText = LOCTEXT("ViewDistanceDescription", "Control the quality of distant world rendering.");
	AntiAliasingText = LOCTEXT("AntiAliasing", "Anti-Aliasing");
	AntiAliasingDescriptionText = LOCTEXT("AntiAliasingDescription", "Control edge smoothing quality.");
	ShadowsText = LOCTEXT("Shadows", "Shadows");
	ShadowsDescriptionText = LOCTEXT("ShadowsDescription", "Control shadow quality.");
	GlobalIlluminationText = LOCTEXT("GlobalIllumination", "Global Illumination");
	GlobalIlluminationDescriptionText = LOCTEXT("GlobalIlluminationDescription", "Control indirect lighting quality.");
	ReflectionsText = LOCTEXT("Reflections", "Reflections");
	ReflectionsDescriptionText = LOCTEXT("ReflectionsDescription", "Control reflection quality.");
	PostProcessingText = LOCTEXT("PostProcessing", "Post Processing");
	PostProcessingDescriptionText = LOCTEXT("PostProcessingDescription", "Control post-processing quality.");
	TexturesText = LOCTEXT("Textures", "Textures");
	TexturesDescriptionText = LOCTEXT("TexturesDescription", "Control texture quality.");
	VisualEffectsText = LOCTEXT("VisualEffects", "Visual Effects");
	VisualEffectsDescriptionText = LOCTEXT("VisualEffectsDescription", "Control visual effects quality.");
	FoliageText = LOCTEXT("Foliage", "Foliage");
	FoliageDescriptionText = LOCTEXT("FoliageDescription", "Control foliage quality.");
	ShadingText = LOCTEXT("Shading", "Shading");
	ShadingDescriptionText = LOCTEXT("ShadingDescription", "Control material shading quality.");
	AdvancedText = LOCTEXT("Advanced", "Advanced");
	VSyncText = LOCTEXT("VSync", "Vertical Sync");
	VSyncDescriptionText = LOCTEXT("VSyncDescription", "Synchronize frame presentation with the display refresh rate.");
	VSyncDisabledUnlessExclusiveText = LOCTEXT("VSyncDisabledUnlessExclusive", "Vertical Sync is only available in exclusive Fullscreen.");
	FrameRateLimitText = LOCTEXT("FrameRateLimit", "Frame Rate Limit");
	FrameRateLimitDescriptionText = LOCTEXT("FrameRateLimitDescription", "Limit the maximum frame rate. Set to 0 for unlimited.");
	DynamicResolutionText = LOCTEXT("DynamicResolution", "Dynamic Resolution");
	DynamicResolutionDescriptionText = LOCTEXT("DynamicResolutionDescription", "Allow the engine to adjust rendering resolution to maintain performance.");
	ResolutionScaleText = LOCTEXT("ResolutionScale", "3D Resolution");
	ResolutionScaleDescriptionText = LOCTEXT("ResolutionScaleDescription", "Scale the internal rendering resolution.");
}

FGSS_DiscreteOptionDefinition UGSS_GameSettingsProvider_Video::MakeOption(const TCHAR* Value, const FText& DisplayName)
{
	FGSS_DiscreteOptionDefinition Option;
	Option.Value = Value;
	Option.DisplayName = DisplayName;
	return Option;
}

TArray<FGSS_DiscreteOptionDefinition> UGSS_GameSettingsProvider_Video::MakeQualityOptions() const
{
	return {
		MakeOption(TEXT("0"), QualityLowText),
		MakeOption(TEXT("1"), QualityMediumText),
		MakeOption(TEXT("2"), QualityHighText),
		MakeOption(TEXT("3"), QualityEpicText),
		MakeOption(TEXT("4"), QualityCinematicText),
	};
}

static void RestrictToPrimaryPlayer(UGSS_GameSetting* Setting)
{
	if (Setting)
	{
		Setting->AddEditCondition(UGSS_WhenPlayingAsPrimaryPlayer::Create(Setting));
	}
}

static void RequireWindowedModeSupport(UGSS_GameSetting* Setting)
{
	if (Setting)
	{
		Setting->AddEditCondition(UGSS_WhenPlatformSupportsWindowedMode::Create(Setting));
		Setting->AddEditCondition(UGSS_WhenPlatformHasTrait::KillIfMissing(Setting, TAG_Platform_Trait_SupportsWindowedMode));
	}
}

static TArray<FGameplayTag> QualityPresetDependencyIds()
{
	return {
		TAG_GSS_Settings_Video_Quality_ViewDistance,
		TAG_GSS_Settings_Video_Quality_AntiAliasing,
		TAG_GSS_Settings_Video_Quality_Shadows,
		TAG_GSS_Settings_Video_Quality_GlobalIllumination,
		TAG_GSS_Settings_Video_Quality_Reflections,
		TAG_GSS_Settings_Video_Quality_PostProcessing,
		TAG_GSS_Settings_Video_Quality_Textures,
		TAG_GSS_Settings_Video_Quality_VisualEffects,
		TAG_GSS_Settings_Video_Quality_Foliage,
		TAG_GSS_Settings_Video_Quality_Shading,
		TAG_GSS_Settings_Video_Advanced_ResolutionScale,
	};
}

UGSS_GameSetting* UGSS_GameSettingsProvider_Video::AddQualitySetting(UGSS_GameSettingsBuilder* Builder, FGameplayTag SettingId, const FText& DisplayName, const FText& Description, FName Getter, FName Setter, UGSS_GameSetting* Parent) const
{
	UGSS_GameSetting* Setting = Builder->AddDiscrete(SettingId, DisplayName, Description, TEXT("3"), MakeQualityOptions(), FGSS_SettingValueAccessor::MakeLocal(Getter, Setter), Parent);
	RestrictToPrimaryPlayer(Setting);
	if (Setting)
	{
		Setting->AddEditDependency(TAG_GSS_Settings_Video_Quality_Overall);
	}
	return Setting;
}

void UGSS_GameSettingsProvider_Video::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder || !IncludedSettings.HasAny())
	{
		return;
	}

	UGSS_GameSetting* VideoScreen = Builder->AddCollection(TAG_GSS_Settings_Video, VideoText, VideoDescriptionText);

	if (IncludedSettings.HasAnyDisplay())
	{
		UGSS_GameSetting* DisplayCollection = Builder->AddCollection(TAG_GSS_Settings_Video_Display, DisplayText, FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bWindowMode)
		{
			UGSS_GameSetting* WindowModeSetting = Builder->AddDiscrete(
				TAG_GSS_Settings_Video_WindowMode,
				this->WindowModeText,
				WindowModeDescriptionText,
				TEXT("WindowedFullscreen"),
				{
					MakeOption(TEXT("Fullscreen"), FullscreenText),
					MakeOption(TEXT("WindowedFullscreen"), WindowedFullscreenText),
					MakeOption(TEXT("Windowed"), WindowedText),
				},
				GSS_MAKE_LOCAL_ACCESSOR(GetFullscreenMode, SetFullscreenMode), DisplayCollection);
			RestrictToPrimaryPlayer(WindowModeSetting);
			RequireWindowedModeSupport(WindowModeSetting);
		}

		if (IncludedSettings.bDisplay)
		{
			UGSS_GameSettingValueDiscrete_Display* DisplaySetting = NewObject<UGSS_GameSettingValueDiscrete_Display>(Builder);
			DisplaySetting->SetSettingId(TAG_GSS_Settings_Video_Display_Monitor);
			DisplaySetting->SetDevName(FName(TEXT("VideoDisplay")));
			DisplaySetting->SetDisplayName(DisplayMonitorText);
			DisplaySetting->SetDescriptionRichText(DisplayMonitorDescriptionText);
			if (Builder->AddRuntimeSetting(DisplaySetting, DisplayCollection))
			{
				RestrictToPrimaryPlayer(DisplaySetting);
				RequireWindowedModeSupport(DisplaySetting);
			}
		}

		if (IncludedSettings.bResolution)
		{
			UGSS_GameSettingValueDiscrete_Resolution* ResolutionSetting = NewObject<UGSS_GameSettingValueDiscrete_Resolution>(Builder);
			ResolutionSetting->SetSettingId(TAG_GSS_Settings_Video_Resolution);
			ResolutionSetting->SetDevName(FName(TEXT("VideoResolution")));
			ResolutionSetting->SetDisplayName(ResolutionText);
			ResolutionSetting->SetDescriptionRichText(ResolutionDescriptionText);
			if (Builder->AddRuntimeSetting(ResolutionSetting, DisplayCollection))
			{
				ResolutionSetting->AddEditCondition(UGSS_WhenSettingHasValue::DisableIfHasAnyValue(
					ResolutionSetting,
					TAG_GSS_Settings_Video_WindowMode,
					TArray<FString>{ TEXT("WindowedFullscreen") },
					ResolutionDisabledWindowedFullscreenText));
				ResolutionSetting->AddEditDependency(TAG_GSS_Settings_Video_WindowMode);
				ResolutionSetting->AddEditDependency(TAG_GSS_Settings_Video_Display_Monitor);
				RestrictToPrimaryPlayer(ResolutionSetting);
				RequireWindowedModeSupport(ResolutionSetting);
			}
		}

		if (IncludedSettings.bBrightness)
		{
			UGSS_GameSettingValueScalar_Brightness* BrightnessSetting = NewObject<UGSS_GameSettingValueScalar_Brightness>(Builder);
			BrightnessSetting->SetSettingId(TAG_GSS_Settings_Video_Brightness);
			BrightnessSetting->SetDevName(FName(TEXT("VideoBrightness")));
			BrightnessSetting->SetDisplayName(BrightnessText);
			BrightnessSetting->SetDescriptionRichText(BrightnessDescriptionText);
			if (Builder->AddRuntimeSetting(BrightnessSetting, DisplayCollection))
			{
				RestrictToPrimaryPlayer(BrightnessSetting);
				BrightnessSetting->AddEditCondition(UGSS_WhenPlatformHasTrait::KillIfMissing(BrightnessSetting, TAG_Platform_Trait_NeedsBrightnessAdjustment));
			}
		}
	}

	if (IncludedSettings.HasAnyQuality())
	{
		UGSS_GameSetting* QualityCollection = Builder->AddCollection(TAG_GSS_Settings_Video_Quality, QualityText, FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bOverallQuality)
		{
			UGSS_GameSettingValueDiscrete_OverallQuality* OverallQualitySetting = NewObject<UGSS_GameSettingValueDiscrete_OverallQuality>(Builder);
			OverallQualitySetting->SetSettingId(TAG_GSS_Settings_Video_Quality_Overall);
			OverallQualitySetting->SetDevName(FName(TEXT("VideoOverallQuality")));
			OverallQualitySetting->SetDisplayName(OverallQualityText);
			OverallQualitySetting->SetDescriptionRichText(OverallQualityDescriptionText);
			OverallQualitySetting->SetAccessor(GSS_MAKE_LOCAL_ACCESSOR(GetOverallScalabilityLevel, SetOverallScalabilityLevel));
			if (Builder->AddRuntimeSetting(OverallQualitySetting, QualityCollection))
			{
				RestrictToPrimaryPlayer(OverallQualitySetting);
				OverallQualitySetting->AddEditDependencies(QualityPresetDependencyIds());
			}
		}

		if (IncludedSettings.bIndividualQuality)
		{
			UGSS_GameSetting* QualityAdvancedCollection = Builder->AddCollection(TAG_GSS_Settings_Video_Quality_Advanced, QualityAdvancedText, QualityAdvancedDescriptionText, QualityCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_ViewDistance, ViewDistanceText, ViewDistanceDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetViewDistanceQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetViewDistanceQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_AntiAliasing, AntiAliasingText, AntiAliasingDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetAntiAliasingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetAntiAliasingQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shadows, ShadowsText, ShadowsDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetShadowQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetShadowQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_GlobalIllumination, GlobalIlluminationText, GlobalIlluminationDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetGlobalIlluminationQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetGlobalIlluminationQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Reflections, ReflectionsText, ReflectionsDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetReflectionQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetReflectionQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_PostProcessing, PostProcessingText, PostProcessingDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetPostProcessingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetPostProcessingQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Textures, TexturesText, TexturesDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetTextureQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetTextureQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_VisualEffects, VisualEffectsText, VisualEffectsDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetVisualEffectQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetVisualEffectQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Foliage, FoliageText, FoliageDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetFoliageQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetFoliageQuality), QualityAdvancedCollection);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shading, ShadingText, ShadingDescriptionText, GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetShadingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetShadingQuality), QualityAdvancedCollection);
		}
	}

	if (IncludedSettings.HasAnyAdvanced())
	{
		UGSS_GameSetting* AdvancedCollection = Builder->AddCollection(TAG_GSS_Settings_Video_Advanced, AdvancedText, FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bVSync)
		{
			UGSS_GameSetting* VSyncSetting = Builder->AddBool(TAG_GSS_Settings_Video_Advanced_VSync, VSyncText, VSyncDescriptionText, false, GSS_MAKE_LOCAL_ACCESSOR(IsVSyncEnabled, SetVSyncEnabled), AdvancedCollection);
			RestrictToPrimaryPlayer(VSyncSetting);
			if (VSyncSetting)
			{
				VSyncSetting->AddEditCondition(UGSS_WhenSettingHasValue::DisableIfLacksValue(
					VSyncSetting,
					TAG_GSS_Settings_Video_WindowMode,
					TArray<FString>{ TEXT("Fullscreen") },
					VSyncDisabledUnlessExclusiveText));
			}
		}
		if (IncludedSettings.bFrameRateLimit)
		{
			RestrictToPrimaryPlayer(Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_FrameRateLimit, FrameRateLimitText, FrameRateLimitDescriptionText, 0.0, 0.0, 360.0, 1.0, GSS_MAKE_LOCAL_ACCESSOR(GetFrameRateLimit, SetFrameRateLimit), AdvancedCollection));
		}
		if (IncludedSettings.bDynamicResolution)
		{
			UGSS_GameSetting* DynamicResolutionSetting = Builder->AddBool(TAG_GSS_Settings_Video_Advanced_DynamicResolution, DynamicResolutionText, DynamicResolutionDescriptionText, false, GSS_MAKE_LOCAL_ACCESSOR(IsDynamicResolutionEnabled, SetDynamicResolutionEnabled), AdvancedCollection);
			RestrictToPrimaryPlayer(DynamicResolutionSetting);
			if (DynamicResolutionSetting)
			{
				DynamicResolutionSetting->AddEditCondition(UGSS_WhenPlatformHasTrait::KillIfMissing(DynamicResolutionSetting, TAG_Platform_Trait_SupportsCustomDynamicResolution));
			}
		}
		if (IncludedSettings.bResolutionScale)
		{
			UGSS_GameSetting* ResolutionScaleSetting = Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_ResolutionScale, ResolutionScaleText, ResolutionScaleDescriptionText, 1.0, 0.1, 1.0, 0.01, GSS_MAKE_LOCAL_ACCESSOR(GetResolutionScaleNormalized, SetResolutionScaleNormalized), AdvancedCollection);
			RestrictToPrimaryPlayer(ResolutionScaleSetting);
			if (ResolutionScaleSetting)
			{
				ResolutionScaleSetting->AddEditDependency(TAG_GSS_Settings_Video_Quality_Overall);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
