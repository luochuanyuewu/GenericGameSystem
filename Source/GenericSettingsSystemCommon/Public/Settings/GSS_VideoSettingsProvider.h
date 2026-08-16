// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_SettingsProvider.h"

#include "GSS_VideoSettingsProvider.generated.h"

/**
 * Registers a ready-to-use baseline of display and graphics settings backed by UGameUserSettings.
 * 注册一组可直接使用、由 UGameUserSettings 支持的显示与图形基础设置。
 *
 * Add this class to GSS Settings Developer Settings -> Startup Provider Classes. Projects may subclass it to
 * remove entries, add platform conditions, or add project-specific display preferences.
 * 请将此类添加到 GSS 项目设置的 Startup Provider Classes。项目可继承它以移除条目、添加平台条件，
 * 或追加项目专用的显示偏好。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_VideoSettingsProvider : public UGSS_SettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder) override;

private:
	static FGSS_SettingValueAccessor MakeLocalAccessor(FName Getter, FName Setter);
	static FGSS_DiscreteOptionDefinition MakeOption(const TCHAR* Value, const FText& DisplayName);
	static TArray<FGSS_DiscreteOptionDefinition> MakeQualityOptions();
	static void AddQualitySetting(UGSS_SettingsBuilder* Builder, FGameplayTag SettingId, const FText& DisplayName, const FText& Description, FName Getter, FName Setter, UGSS_GameSetting* Parent);
};
