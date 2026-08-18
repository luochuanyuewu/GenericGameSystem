// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#pragma once

#include "Settings/GSS_SettingsShared.h"
#include "GSS_CommonSettingsShared.generated.h"

/**
 * Base Shared Settings preset for the standard GSS Common providers.
 * 标准 GSS Common Provider 使用的 Shared Settings 预制基类。
 *
 * This class only stores portable preference values. Projects should derive a Blueprint or C++ class,
 * override ApplySettings, and apply the committed preferences to their own audio, input, and accessibility systems.
 * 本类仅存储可携带的偏好值。项目应创建蓝图或 C++ 子类，重写 ApplySettings，
 * 再将已提交的偏好应用到自身的音频、输入和无障碍系统。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_CommonSettingsShared : public UGSS_SettingsShared
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") float GetMasterVolume() const { return MasterVolume; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetMasterVolume(float Value) { MasterVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetMusicVolume(float Value) { MusicVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") float GetSoundEffectsVolume() const { return SoundEffectsVolume; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetSoundEffectsVolume(float Value) { SoundEffectsVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") float GetDialogueVolume() const { return DialogueVolume; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetDialogueVolume(float Value) { DialogueVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") float GetVoiceChatVolume() const { return VoiceChatVolume; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetVoiceChatVolume(float Value) { VoiceChatVolume = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Audio") bool GetMuteAllAudio() const { return bMuteAllAudio; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Audio") void SetMuteAllAudio(bool Value) { bMuteAllAudio = Value; }

	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetMouseHorizontalSensitivity() const { return MouseHorizontalSensitivity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetMouseHorizontalSensitivity(float Value) { MouseHorizontalSensitivity = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetMouseVerticalSensitivity() const { return MouseVerticalSensitivity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetMouseVerticalSensitivity(float Value) { MouseVerticalSensitivity = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") bool GetInvertMouseVerticalAxis() const { return bInvertMouseVerticalAxis; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetInvertMouseVerticalAxis(bool Value) { bInvertMouseVerticalAxis = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") bool GetInvertMouseHorizontalAxis() const { return bInvertMouseHorizontalAxis; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetInvertMouseHorizontalAxis(bool Value) { bInvertMouseHorizontalAxis = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") bool GetGamepadVibration() const { return bGamepadVibration; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetGamepadVibration(bool Value) { bGamepadVibration = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") bool GetInvertGamepadVerticalAxis() const { return bInvertGamepadVerticalAxis; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetInvertGamepadVerticalAxis(bool Value) { bInvertGamepadVerticalAxis = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") bool GetInvertGamepadHorizontalAxis() const { return bInvertGamepadHorizontalAxis; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetInvertGamepadHorizontalAxis(bool Value) { bInvertGamepadHorizontalAxis = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetGamepadLookSensitivity() const { return GamepadLookSensitivity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetGamepadLookSensitivity(float Value) { GamepadLookSensitivity = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetGamepadAimSensitivity() const { return GamepadAimSensitivity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetGamepadAimSensitivity(float Value) { GamepadAimSensitivity = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetMoveStickDeadZone() const { return MoveStickDeadZone; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetMoveStickDeadZone(float Value) { MoveStickDeadZone = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Input") float GetLookStickDeadZone() const { return LookStickDeadZone; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Input") void SetLookStickDeadZone(float Value) { LookStickDeadZone = Value; }

	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") bool GetSubtitlesEnabled() const { return bSubtitlesEnabled; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetSubtitlesEnabled(bool Value) { bSubtitlesEnabled = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") float GetSubtitleTextScale() const { return SubtitleTextScale; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetSubtitleTextScale(float Value) { SubtitleTextScale = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") FString GetSubtitleTextColor() const { return SubtitleTextColor; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetSubtitleTextColor(const FString& Value) { SubtitleTextColor = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") bool GetSubtitleBorderEnabled() const { return bSubtitleBorderEnabled; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetSubtitleBorderEnabled(bool Value) { bSubtitleBorderEnabled = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") float GetSubtitleBackgroundOpacity() const { return SubtitleBackgroundOpacity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetSubtitleBackgroundOpacity(float Value) { SubtitleBackgroundOpacity = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") FString GetColorVisionMode() const { return ColorVisionMode; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetColorVisionMode(const FString& Value) { ColorVisionMode = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") float GetColorVisionStrength() const { return ColorVisionStrength; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetColorVisionStrength(float Value) { ColorVisionStrength = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") bool GetReduceCameraMotion() const { return bReduceCameraMotion; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetReduceCameraMotion(bool Value) { bReduceCameraMotion = Value; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Accessibility") float GetScreenShakeIntensity() const { return ScreenShakeIntensity; }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Accessibility") void SetScreenShakeIntensity(float Value) { ScreenShakeIntensity = Value; }

private:
	UPROPERTY(SaveGame) float MasterVolume = 1.0f;
	UPROPERTY(SaveGame) float MusicVolume = 1.0f;
	UPROPERTY(SaveGame) float SoundEffectsVolume = 1.0f;
	UPROPERTY(SaveGame) float DialogueVolume = 1.0f;
	UPROPERTY(SaveGame) float VoiceChatVolume = 1.0f;
	UPROPERTY(SaveGame) bool bMuteAllAudio = false;
	UPROPERTY(SaveGame) float MouseHorizontalSensitivity = 1.0f;
	UPROPERTY(SaveGame) float MouseVerticalSensitivity = 1.0f;
	UPROPERTY(SaveGame) bool bInvertMouseVerticalAxis = false;
	UPROPERTY(SaveGame) bool bInvertMouseHorizontalAxis = false;
	UPROPERTY(SaveGame) bool bGamepadVibration = true;
	UPROPERTY(SaveGame) bool bInvertGamepadVerticalAxis = false;
	UPROPERTY(SaveGame) bool bInvertGamepadHorizontalAxis = false;
	UPROPERTY(SaveGame) float GamepadLookSensitivity = 1.0f;
	UPROPERTY(SaveGame) float GamepadAimSensitivity = 1.0f;
	UPROPERTY(SaveGame) float MoveStickDeadZone = 0.1f;
	UPROPERTY(SaveGame) float LookStickDeadZone = 0.1f;
	UPROPERTY(SaveGame) bool bSubtitlesEnabled = true;
	UPROPERTY(SaveGame) float SubtitleTextScale = 1.0f;
	UPROPERTY(SaveGame) FString SubtitleTextColor = TEXT("White");
	UPROPERTY(SaveGame) bool bSubtitleBorderEnabled = true;
	UPROPERTY(SaveGame) float SubtitleBackgroundOpacity = 0.5f;
	UPROPERTY(SaveGame) FString ColorVisionMode = TEXT("None");
	UPROPERTY(SaveGame) float ColorVisionStrength = 1.0f;
	UPROPERTY(SaveGame) bool bReduceCameraMotion = false;
	UPROPERTY(SaveGame) float ScreenShakeIntensity = 1.0f;
};
