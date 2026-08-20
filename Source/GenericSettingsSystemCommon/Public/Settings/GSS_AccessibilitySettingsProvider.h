// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_AccessibilitySettingsProvider.generated.h"

/**
 * Registers common local accessibility preferences under GSS.Settings.Accessibility.
 * 在 GSS.Settings.Accessibility 下注册常见的本地无障碍偏好。
 *
 * Values are framework-neutral preferences. Projects apply them to their subtitle, UI, camera and post-process
 * implementations by querying or subscribing to the corresponding setting tags.
 * 数值是与框架无关的偏好。项目通过查询或订阅对应设置 Tag，将其应用到字幕、UI、相机和后处理实现。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_AccessibilitySettingsProvider : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;

private:
	static FGSS_DiscreteOptionDefinition MakeOption(const TCHAR* Value, const FText& DisplayName);
};
