// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_VideoSettingsProvider.generated.h"

/**
 * Selects which Common Video rows a provider instance registers.
 * 选择该 Provider 实例会注册哪些 Common Video 行。
 *
 * Defaults keep the previous full desktop baseline. Create a Blueprint subclass and uncheck rows
 * instead of copying RegisterSettings just to hide a single item.
 * 默认值保持原先的完整桌面基线。若只需隐藏个别项，请创建蓝图子类并取消勾选，而不必复制 RegisterSettings。
 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEMCOMMON_API FGSS_VideoSettingsInclusion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	bool bWindowMode = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	bool bResolution = true;

	/** Engine display gamma slider persisted in GameUserSettings.ini. / 引擎显示 Gamma 滑条，持久化到 GameUserSettings.ini。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	bool bBrightness = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
	bool bOverallQuality = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality")
	bool bIndividualQuality = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	bool bVSync = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	bool bFrameRateLimit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	bool bDynamicResolution = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced")
	bool bResolutionScale = true;

	bool HasAnyDisplay() const { return bWindowMode || bResolution || bBrightness; }
	bool HasAnyQuality() const { return bOverallQuality || bIndividualQuality; }
	bool HasAnyAdvanced() const { return bVSync || bFrameRateLimit || bDynamicResolution || bResolutionScale; }
	bool HasAny() const { return HasAnyDisplay() || HasAnyQuality() || HasAnyAdvanced(); }
};

/**
 * Registers a ready-to-use baseline of display and graphics settings backed by UGameUserSettings.
 * 注册一组可直接使用、由 UGameUserSettings 支持的显示与图形基础设置。
 *
 * Add this class to GSS Settings Developer Settings -> Startup Provider Classes. Projects may subclass it to
 * uncheck IncludedSettings rows, add platform conditions, or add project-specific display preferences.
 * 请将此类添加到 GSS 项目设置的 Startup Provider Classes。项目可继承它以取消 IncludedSettings 中的条目、
 * 添加平台条件，或追加项目专用的显示偏好。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_VideoSettingsProvider : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;

	/** Rows registered by this instance. Uncheck to omit a Common Video item. / 本实例注册的行；取消勾选即可省略对应 Common Video 项。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSS|Settings")
	FGSS_VideoSettingsInclusion IncludedSettings;

private:
	static FGSS_DiscreteOptionDefinition MakeOption(const TCHAR* Value, const FText& DisplayName);
	static TArray<FGSS_DiscreteOptionDefinition> MakeQualityOptions();
	static void AddQualitySetting(UGSS_GameSettingsBuilder* Builder, FGameplayTag SettingId, const FText& DisplayName, const FText& Description, FName Getter, FName Setter, UGSS_GameSetting* Parent);
};
