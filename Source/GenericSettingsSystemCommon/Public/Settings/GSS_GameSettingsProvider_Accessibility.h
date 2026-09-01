// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_GameSettingsProvider_Accessibility.generated.h"


/**
 * Registers common local accessibility preferences under GSS.Settings.Accessibility.
 * 在 GSS.Settings.Accessibility 下注册常见的本地无障碍偏好。
 *
 * Values are framework-neutral preferences. Projects apply them to their subtitle, UI, camera and post-process
 * implementations by querying or subscribing to the corresponding setting tags.
 * 数值是与框架无关的偏好。项目通过查询或订阅对应设置 Tag，将其应用到字幕、UI、相机和后处理实现。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingsProvider_Accessibility : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	UGSS_GameSettingsProvider_Accessibility();

	/** Default localized text registered by this Provider. Every field can be overridden by a Blueprint child. / 本 Provider 注册的默认本地化文本；每个字段都可由蓝图子类覆盖。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText AccessibilityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindDeuteranopeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindModeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindNoneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindProtanopeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindStrengthText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorBlindTritanopeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ColorVisionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MotionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ReduceCameraMotionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText ScreenShakeIntensityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleBackgroundOpacityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleBorderText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleColorCyanText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleColorWhiteText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleColorYellowText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleTextColorText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitleTextScaleText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitlesText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SubtitlesEnabledText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText AccessibilityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SubtitlesEnabledDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SubtitleTextScaleDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SubtitleTextColorDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SubtitleBorderDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SubtitleBackgroundOpacityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText ColorBlindModeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText ColorBlindStrengthDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText ReduceCameraMotionDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText ScreenShakeIntensityDescriptionText;

	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;

private:
	static FGSS_DiscreteOptionDefinition MakeOption(const TCHAR* Value, const FText& DisplayName);
};
