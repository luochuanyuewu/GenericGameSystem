// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_VideoSettingsProvider.h"

#include "NativeGameplayTags.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingValueDiscrete_OverallQuality.h"
#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_VideoSettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_VideoSettingsProvider"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video, "GSS.Settings.Video");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Display, "GSS.Settings.Video.Display");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_WindowMode, "GSS.Settings.Video.WindowMode");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Video_Resolution, "GSS.Settings.Video.Display.Resolution");
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

FGSS_SettingValueAccessor UGSS_VideoSettingsProvider::MakeLocalAccessor(FName Getter, FName Setter)
{
	FGSS_SettingValueAccessor Accessor;
	Accessor.Source = EGSS_SettingValueSource::Local;
	Accessor.GetterFunction = Getter;
	Accessor.SetterFunction = Setter;
	return Accessor;
}

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

void UGSS_VideoSettingsProvider::AddQualitySetting(UGSS_SettingsBuilder* Builder, FGameplayTag SettingId, const FText& DisplayName, const FText& Description, FName Getter, FName Setter, UGSS_GameSetting* Parent)
{
	Builder->AddDiscrete(SettingId, DisplayName, Description, TEXT("3"), MakeQualityOptions(), MakeLocalAccessor(Getter, Setter), Parent);
}

void UGSS_VideoSettingsProvider::RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* VideoScreen = Builder->AddCollection(TAG_GSS_Settings_Video, LOCTEXT("Video", "Video"), LOCTEXT("VideoDescription", "Configure display and graphics preferences."));
	UGSS_GameSetting* Display = Builder->AddCollection(TAG_GSS_Settings_Video_Display, LOCTEXT("Display", "Display"), FText::GetEmpty(), VideoScreen);
	UGSS_GameSetting* WindowMode = Builder->AddDiscrete(
		TAG_GSS_Settings_Video_WindowMode,
		LOCTEXT("WindowMode", "Window Mode"),
		LOCTEXT("WindowModeDescription", "Choose how the game window is displayed."),
		TEXT("1"),
		{
			MakeOption(TEXT("0"), LOCTEXT("Fullscreen", "Fullscreen")),
			MakeOption(TEXT("1"), LOCTEXT("WindowedFullscreen", "Windowed Fullscreen")),
			MakeOption(TEXT("2"), LOCTEXT("Windowed", "Windowed")),
		},
		MakeLocalAccessor(TEXT("GetFullscreenMode"), TEXT("SetFullscreenMode")), Display);

	UGSS_GameSettingValueDiscrete_Resolution* Resolution = NewObject<UGSS_GameSettingValueDiscrete_Resolution>(Builder);
	Resolution->SetSettingId(TAG_GSS_Settings_Video_Resolution);
	Resolution->SetDevName(FName(TEXT("VideoResolution")));
	Resolution->SetDisplayName(LOCTEXT("Resolution", "Resolution"));
	Resolution->SetDescriptionRichText(LOCTEXT("ResolutionDescription", "Choose the display output resolution."));
	if (Builder->AddRuntimeSetting(Resolution, Display))
	{
		Resolution->AddEditDependency(WindowMode);
	}

	UGSS_GameSetting* Quality = Builder->AddCollection(TAG_GSS_Settings_Video_Quality, LOCTEXT("Quality", "Graphics Quality"), FText::GetEmpty(), VideoScreen);
	UGSS_GameSettingValueDiscrete_OverallQuality* OverallQuality = NewObject<UGSS_GameSettingValueDiscrete_OverallQuality>(Builder);
	OverallQuality->SetSettingId(TAG_GSS_Settings_Video_Quality_Overall);
	OverallQuality->SetDevName(FName(TEXT("VideoOverallQuality")));
	OverallQuality->SetDisplayName(LOCTEXT("OverallQuality", "Quality Preset"));
	OverallQuality->SetDescriptionRichText(LOCTEXT("OverallQualityDescription", "Apply one graphics quality level to all scalability groups."));
	OverallQuality->SetAccessor(MakeLocalAccessor(TEXT("GetOverallScalabilityLevel"), TEXT("SetOverallScalabilityLevel")));
	Builder->AddRuntimeSetting(OverallQuality, Quality);

	UGSS_GameSetting* QualityAdvanced = Builder->AddCollection(TAG_GSS_Settings_Video_Quality_Advanced, LOCTEXT("QualityAdvanced", "Individual Quality"), LOCTEXT("QualityAdvancedDescription", "Fine-tune individual graphics features."), Quality);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_ViewDistance, LOCTEXT("ViewDistance", "View Distance"), LOCTEXT("ViewDistanceDescription", "Control the quality of distant world rendering."), TEXT("GetViewDistanceQuality"), TEXT("SetViewDistanceQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_AntiAliasing, LOCTEXT("AntiAliasing", "Anti-Aliasing"), LOCTEXT("AntiAliasingDescription", "Control edge smoothing quality."), TEXT("GetAntiAliasingQuality"), TEXT("SetAntiAliasingQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shadows, LOCTEXT("Shadows", "Shadows"), LOCTEXT("ShadowsDescription", "Control shadow quality."), TEXT("GetShadowQuality"), TEXT("SetShadowQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_GlobalIllumination, LOCTEXT("GlobalIllumination", "Global Illumination"), LOCTEXT("GlobalIlluminationDescription", "Control indirect lighting quality."), TEXT("GetGlobalIlluminationQuality"), TEXT("SetGlobalIlluminationQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Reflections, LOCTEXT("Reflections", "Reflections"), LOCTEXT("ReflectionsDescription", "Control reflection quality."), TEXT("GetReflectionQuality"), TEXT("SetReflectionQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_PostProcessing, LOCTEXT("PostProcessing", "Post Processing"), LOCTEXT("PostProcessingDescription", "Control post-processing quality."), TEXT("GetPostProcessingQuality"), TEXT("SetPostProcessingQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Textures, LOCTEXT("Textures", "Textures"), LOCTEXT("TexturesDescription", "Control texture quality."), TEXT("GetTextureQuality"), TEXT("SetTextureQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_VisualEffects, LOCTEXT("VisualEffects", "Visual Effects"), LOCTEXT("VisualEffectsDescription", "Control visual effects quality."), TEXT("GetVisualEffectQuality"), TEXT("SetVisualEffectQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Foliage, LOCTEXT("Foliage", "Foliage"), LOCTEXT("FoliageDescription", "Control foliage quality."), TEXT("GetFoliageQuality"), TEXT("SetFoliageQuality"), QualityAdvanced);
	AddQualitySetting(Builder, TAG_GSS_Settings_Video_Quality_Shading, LOCTEXT("Shading", "Shading"), LOCTEXT("ShadingDescription", "Control material shading quality."), TEXT("GetShadingQuality"), TEXT("SetShadingQuality"), QualityAdvanced);

	UGSS_GameSetting* Advanced = Builder->AddCollection(TAG_GSS_Settings_Video_Advanced, LOCTEXT("Advanced", "Advanced"), FText::GetEmpty(), VideoScreen);
	Builder->AddBool(TAG_GSS_Settings_Video_Advanced_VSync, LOCTEXT("VSync", "Vertical Sync"), LOCTEXT("VSyncDescription", "Synchronize frame presentation with the display refresh rate."), false, MakeLocalAccessor(TEXT("IsVSyncEnabled"), TEXT("SetVSyncEnabled")), Advanced);
	Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_FrameRateLimit, LOCTEXT("FrameRateLimit", "Frame Rate Limit"), LOCTEXT("FrameRateLimitDescription", "Limit the maximum frame rate. Set to 0 for unlimited."), 0.0, 0.0, 360.0, 1.0, MakeLocalAccessor(TEXT("GetFrameRateLimit"), TEXT("SetFrameRateLimit")), Advanced);
	Builder->AddBool(TAG_GSS_Settings_Video_Advanced_DynamicResolution, LOCTEXT("DynamicResolution", "Dynamic Resolution"), LOCTEXT("DynamicResolutionDescription", "Allow the engine to adjust rendering resolution to maintain performance."), false, MakeLocalAccessor(TEXT("IsDynamicResolutionEnabled"), TEXT("SetDynamicResolutionEnabled")), Advanced);
	Builder->AddScalar(TAG_GSS_Settings_Video_Advanced_ResolutionScale, LOCTEXT("ResolutionScale", "3D Resolution"), LOCTEXT("ResolutionScaleDescription", "Scale the internal rendering resolution."), 1.0, 0.1, 1.0, 0.01, MakeLocalAccessor(TEXT("GetResolutionScaleNormalized"), TEXT("SetResolutionScaleNormalized")), Advanced);
}

#undef LOCTEXT_NAMESPACE
