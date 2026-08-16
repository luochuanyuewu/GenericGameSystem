// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GSS_SettingsDeveloperSettings.generated.h"

class UGSS_SettingsDefinitionAsset;
class UGSS_SettingsProvider;
class UGSS_SettingsShared;

/** Project-wide GSS configuration shown in Project Settings. / 在项目设置中显示的 GSS 项目级配置。 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Generic Settings System"))
class GENERICSETTINGSSYSTEM_API UGSS_SettingsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Optional root asset loaded and built for every LocalPlayer subsystem. / 每个 LocalPlayer 子系统加载并构建的可选根资产。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings")
	TSoftObjectPtr<UGSS_SettingsDefinitionAsset> RootDefinition;

	/** Shared preference class loaded separately for every LocalPlayer. / 为每个 LocalPlayer 单独加载的共享偏好类。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings", meta = (AllowAbstract = false))
	TSubclassOf<UGSS_SettingsShared> SharedSettingsClass;

	/** Provider classes registered automatically for every LocalPlayer registry. / 为每个 LocalPlayer Registry 自动注册的 Provider 类。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings", meta = (AllowAbstract = false))
	TArray<TSoftClassPtr<UGSS_SettingsProvider>> StartupProviderClasses;

	/** Stable slot name used by the LocalPlayer SaveGame API. / LocalPlayer SaveGame API 使用的稳定存档槽名。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings")
	FString SharedSettingsSlotName = TEXT("GSS_SettingsShared");
};
