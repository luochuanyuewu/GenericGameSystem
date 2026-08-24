// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_VideoSettingsProvider.h"

#include "NativeGameplayTags.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/EditCondition/GSS_WhenPlatformSupportsWindowedMode.h"
#include "Settings/EditCondition/GSS_WhenPlayingAsPrimaryPlayer.h"
#include "Settings/EditCondition/GSS_WhenSettingHasValue.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingValueDiscrete_OverallQuality.h"
#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"
#include "Settings/GSS_GameSettingValueScalar_Brightness.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_VideoSettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_VideoSettingsProvider"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video, "GSS.Settings.Video");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Display, "GSS.Settings.Video.Display");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_WindowMode, "GSS.Settings.Video.WindowMode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Resolution, "GSS.Settings.Video.Display.Resolution");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Brightness, "GSS.Settings.Video.Display.Brightness");
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

FGSS_DiscreteOptionDefinition UGSS_VideoSettingsProvider::MakeOption(const TCHAR* Value, const FText& DisplayName)
{
	FGSS_DiscreteOptionDefinition Option;
	Option.Value = Value;
	Option.DisplayName = DisplayName;
	return Option;
}

TArray<FGSS_DiscreteOptionDefinition> UGSS_VideoSettingsProvider::MakeQualityOptions()
{
	return {
		MakeOption(TEXT("0"), LOCTEXT("QualityLow", "Low")),
		MakeOption(TEXT("1"), LOCTEXT("QualityMedium", "Medium")),
		MakeOption(TEXT("2"), LOCTEXT("QualityHigh", "High")),
		MakeOption(TEXT("3"), LOCTEXT("QualityEpic", "Epic")),
		MakeOption(TEXT("4"), LOCTEXT("QualityCinematic", "Cinematic")),
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

UGSS_GameSetting* UGSS_VideoSettingsProvider::AddQualitySetting(UGSS_GameSettingsBuilder* Builder, FGameplayTag SettingId, const FText& DisplayName, const FText& Description, FName Getter, FName Setter, UGSS_GameSetting* Parent)
{
	UGSS_GameSetting* Setting = Builder->AddDiscrete(SettingId, DisplayName, Description, TEXT("3"), MakeQualityOptions(), FGSS_SettingValueAccessor::MakeLocal(Getter, Setter), Parent);
	RestrictToPrimaryPlayer(Setting);
	if (Setting)
	{
		Setting->AddEditDependency(TAG_GSS_Settings_Video_Quality_Overall);
	}
	return Setting;
}

void UGSS_VideoSettingsProvider::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder || !IncludedSettings.HasAny())
	{
		return;
	}

	UGSS_GameSetting* VideoScreen = Builder->AddCollection(TAG_GSS_Settings_Video, LOCTEXT("Video", "Video"), LOCTEXT("VideoDescription", "Configure display and graphics preferences."));

	if (IncludedSettings.HasAnyDisplay())
	{
		UGSS_GameSetting* Display = Builder->AddCollection(TAG_GSS_Settings_Video_Display, LOCTEXT("Display", "Display"), FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bWindowMode)
		{
			UGSS_GameSetting* WindowMode = Builder->AddDiscrete(
				TAG_GSS_Settings_Video_WindowMode,
				LOCTEXT("WindowMode", "Window Mode"),
				LOCTEXT("WindowModeDescription", "Choose how the game window is displayed."),
				TEXT("WindowedFullscreen"),
				{
					MakeOption(TEXT("Fullscreen"), LOCTEXT("Fullscreen", "Fullscreen")),
					MakeOption(TEXT("WindowedFullscreen"), LOCTEXT("WindowedFullscreen", "Windowed Fullscreen")),
					MakeOption(TEXT("Windowed"), LOCTEXT("Windowed", "Windowed")),
				},
				GSS_MAKE_LOCAL_ACCESSOR(GetFullscreenMode, SetFullscreenMode), Display);
			RestrictToPrimaryPlayer(WindowMode);
			RequireWindowedModeSupport(WindowMode);
		}

		if (IncludedSettings.bResolution)
		{
			UGSS_GameSettingValueDiscrete_Resolution* Resolution = NewObject<UGSS_GameSettingValueDiscrete_Resolution>(Builder);
			Resolution->SetSettingId(TAG_GSS_Settings_Video_Resolution);
			Resolution->SetDevName(FName(TEXT("VideoResolution")));
			Resolution->SetDisplayName(LOCTEXT("Resolution", "Resolution"));
			Resolution->SetDescriptionRichText(LOCTEXT("ResolutionDescription", "Choose the display output resolution."));
			if (Builder->AddRuntimeSetting(Resolution, Display))
			{
				Resolution->AddEditCondition(UGSS_WhenSettingHasValue::DisableIfHasAnyValue(
					Resolution,
					TAG_GSS_Settings_Video_WindowMode,
					TArray<FString>{ TEXT("WindowedFullscreen") },
					LOCTEXT("ResolutionDisabledWindowedFullscreen", "Resolution is controlled by the desktop in Windowed Fullscreen.")));
				RestrictToPrimaryPlayer(Resolution);
				RequireWindowedModeSupport(Resolution);
			}
		}

		if (IncludedSettings.bBrightness)
		{
			UGSS_GameSettingValueScalar_Brightness* Brightness = NewObject<UGSS_GameSettingValueScalar_Brightness>(Builder);
			Brightness->SetSettingId(TAG_GSS_Settings_Video_Brightness);
			Brightness->SetDevName(FName(TEXT("VideoBrightness")));
			Brightness->SetDisplayName(LOCTEXT("Brightness", "Brightness"));
			Brightness->SetDescriptionRichText(LOCTEXT("BrightnessDescription", "Adjust display brightness."));
			if (Builder->AddRuntimeSetting(Brightness, Display))
			{
				RestrictToPrimaryPlayer(Brightness);
			}
		}
	}

	if (IncludedSettings.HasAnyQuality())
	{
		UGSS_GameSetting* Quality = Builder->AddCollection(TAG_GSS_Settings_Video_Quality, LOCTEXT("Quality", "Graphics Quality"), FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bOverallQuality)
		{
			UGSS_GameSettingValueDiscrete_OverallQuality* OverallQuality = NewObject<UGSS_GameSettingValueDiscrete_OverallQuality>(Builder);
			OverallQuality->SetSettingId(TAG_GSS_Settings_Video_Quality_Overall);
			OverallQuality->SetDevName(FName(TEXT("VideoOverallQuality")));
			OverallQuality->SetDisplayName(LOCTEXT("OverallQuality", "Quality Preset"));
			OverallQuality->SetDescriptionRichText(LOCTEXT("OverallQualityDescription", "Apply one graphics quality level to all scalability groups."));
			OverallQuality->SetAccessor(GSS_MAKE_LOCAL_ACCESSOR(GetOverallScalabilityLevel, SetOverallScalabilityLevel));
			if (Builder->AddRuntimeSetting(OverallQuality, Quality))
			{
				RestrictToPrimaryPlayer(OverallQuality);
				OverallQuality->AddEditDependencies(QualityPresetDependencyIds());
			}
		}

		if (IncludedSettings.bIndividualQuality)
		{
			UGSS_GameSetting* QualityAdvanced = Builder->AddCollection(TAG_GSS_Settings_Video_Quality_Advanced, LOCTEXT("QualityAdvanced", "Individual Quality"), LOCTEXT("QualityAdvancedDescription", "Fine-tune individual graphics features."), Quality);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_ViewDistance, LOCTEXT("ViewDistance", "View Distance"), LOCTEXT("ViewDistanceDescription", "Control the quality of distant world rendering."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetViewDistanceQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetViewDistanceQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_AntiAliasing, LOCTEXT("AntiAliasing", "Anti-Aliasing"), LOCTEXT("AntiAliasingDescription", "Control edge smoothing quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetAntiAliasingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetAntiAliasingQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shadows, LOCTEXT("Shadows", "Shadows"), LOCTEXT("ShadowsDescription", "Control shadow quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetShadowQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetShadowQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_GlobalIllumination, LOCTEXT("GlobalIllumination", "Global Illumination"), LOCTEXT("GlobalIlluminationDescription", "Control indirect lighting quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetGlobalIlluminationQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetGlobalIlluminationQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Reflections, LOCTEXT("Reflections", "Reflections"), LOCTEXT("ReflectionsDescription", "Control reflection quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetReflectionQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetReflectionQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_PostProcessing, LOCTEXT("PostProcessing", "Post Processing"), LOCTEXT("PostProcessingDescription", "Control post-processing quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetPostProcessingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetPostProcessingQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Textures, LOCTEXT("Textures", "Textures"), LOCTEXT("TexturesDescription", "Control texture quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetTextureQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetTextureQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_VisualEffects, LOCTEXT("VisualEffects", "Visual Effects"), LOCTEXT("VisualEffectsDescription", "Control visual effects quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetVisualEffectQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetVisualEffectQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Foliage, LOCTEXT("Foliage", "Foliage"), LOCTEXT("FoliageDescription", "Control foliage quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetFoliageQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetFoliageQuality), QualityAdvanced);
			AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shading, LOCTEXT("Shading", "Shading"), LOCTEXT("ShadingDescription", "Control material shading quality."), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, GetShadingQuality), GET_FUNCTION_NAME_CHECKED(UGameUserSettings, SetShadingQuality), QualityAdvanced);
		}
	}

	if (IncludedSettings.HasAnyAdvanced())
	{
		UGSS_GameSetting* Advanced = Builder->AddCollection(TAG_GSS_Settings_Video_Advanced, LOCTEXT("Advanced", "Advanced"), FText::GetEmpty(), VideoScreen);
		if (IncludedSettings.bVSync)
		{
			UGSS_GameSetting* VSync = Builder->AddBool(TAG_GSS_Settings_Video_Advanced_VSync, LOCTEXT("VSync", "Vertical Sync"), LOCTEXT("VSyncDescription", "Synchronize frame presentation with the display refresh rate."), false, GSS_MAKE_LOCAL_ACCESSOR(IsVSyncEnabled, SetVSyncEnabled), Advanced);
			RestrictToPrimaryPlayer(VSync);
			if (VSync)
			{
				VSync->AddEditCondition(UGSS_WhenSettingHasValue::DisableIfLacksValue(
					VSync,
					TAG_GSS_Settings_Video_WindowMode,
					TArray<FString>{ TEXT("Fullscreen") },
					LOCTEXT("VSyncDisabledUnlessExclusive", "Vertical Sync is only available in exclusive Fullscreen.")));
			}
		}
		if (IncludedSettings.bFrameRateLimit)
		{
			RestrictToPrimaryPlayer(Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_FrameRateLimit, LOCTEXT("FrameRateLimit", "Frame Rate Limit"), LOCTEXT("FrameRateLimitDescription", "Limit the maximum frame rate. Set to 0 for unlimited."), 0.0, 0.0, 360.0, 1.0, GSS_MAKE_LOCAL_ACCESSOR(GetFrameRateLimit, SetFrameRateLimit), Advanced));
		}
		if (IncludedSettings.bDynamicResolution)
		{
			RestrictToPrimaryPlayer(Builder->AddBool(TAG_GSS_Settings_Video_Advanced_DynamicResolution, LOCTEXT("DynamicResolution", "Dynamic Resolution"), LOCTEXT("DynamicResolutionDescription", "Allow the engine to adjust rendering resolution to maintain performance."), false, GSS_MAKE_LOCAL_ACCESSOR(IsDynamicResolutionEnabled, SetDynamicResolutionEnabled), Advanced));
		}
		if (IncludedSettings.bResolutionScale)
		{
			UGSS_GameSetting* ResolutionScale = Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_ResolutionScale, LOCTEXT("ResolutionScale", "3D Resolution"), LOCTEXT("ResolutionScaleDescription", "Scale the internal rendering resolution."), 1.0, 0.1, 1.0, 0.01, GSS_MAKE_LOCAL_ACCESSOR(GetResolutionScaleNormalized, SetResolutionScaleNormalized), Advanced);
			RestrictToPrimaryPlayer(ResolutionScale);
			if (ResolutionScale)
			{
				ResolutionScale->AddEditDependency(TAG_GSS_Settings_Video_Quality_Overall);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
