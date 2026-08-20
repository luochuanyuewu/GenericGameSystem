// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_AudioSettingsProvider.generated.h"

/**
 * Registers common local audio preferences under GSS.Settings.Audio.
 * 在 GSS.Settings.Audio 下注册常见的本地音频偏好。
 *
 * These values are stored on Shared Settings rather than bound to a particular audio middleware. Subscribe
 * to their setting tags from the project's audio system and apply the values to SoundMixes, Control Buses or a
 * middleware integration.
 * 这些值有意存储在 Shared Settings 中，而不绑定特定音频中间件。项目音频系统应订阅对应设置 Tag，
 * 并将数值应用到 SoundMix、Control Bus 或中间件集成。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_AudioSettingsProvider : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;
};
