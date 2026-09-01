// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_GameSettingsProvider_Audio.generated.h"


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
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingsProvider_Audio : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	UGSS_GameSettingsProvider_Audio();

	/** Default localized text registered by this Provider. Every field can be overridden by a Blueprint child. / 本 Provider 注册的默认本地化文本；每个字段都可由蓝图子类覆盖。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText AudioText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText DialogueVolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MasterVolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MusicVolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MuteAllText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SFXVolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText SoundText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText VoiceChatVolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText VolumeText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText AudioDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MasterVolumeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MusicVolumeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText SFXVolumeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText DialogueVolumeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText VoiceChatVolumeDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MuteAllDescriptionText;

	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;
};
