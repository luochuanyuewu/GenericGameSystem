// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GSS_DeveloperSettings.generated.h"

class UGSS_GameSettingsDefinitionAsset;
class UGSS_GameSettingsProvider;
class UGSS_SettingsShared;

/** Project-wide GSS configuration shown in Project Settings. / 在项目设置中显示的 GSS 项目级配置。 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Generic Setting System Developer Settings"))
class GENERICSETTINGSSYSTEM_API UGSS_DeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Optional root asset loaded and built for every LocalPlayer subsystem. / 每个 LocalPlayer 子系统加载并构建的可选根资产。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings")
	TSoftObjectPtr<UGSS_GameSettingsDefinitionAsset> RootDefinition;

	/** Shared preference class loaded separately for every LocalPlayer. / 为每个 LocalPlayer 单独加载的共享偏好类。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings", meta = (AllowAbstract = false))
	TSoftClassPtr<UGSS_SettingsShared> SharedSettingsClass;

	/** Loads SharedSettingsClass, or UGSS_SettingsShared when the soft reference is empty. / 加载 SharedSettingsClass；软引用为空时回退到 UGSS_SettingsShared。 */
	UClass* ResolveSharedSettingsClass() const;

	/** Provider classes registered automatically for every LocalPlayer registry. / 为每个 LocalPlayer Registry 自动注册的 Provider 类。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings", meta = (AllowAbstract = false))
	TArray<TSoftClassPtr<UGSS_GameSettingsProvider>> StartupProviderClasses;

	/** Stable slot name used by the LocalPlayer SaveGame API. / LocalPlayer SaveGame API 使用的稳定存档槽名。 */
	UPROPERTY(Config, EditAnywhere, Category = "GSS|Settings")
	FString SharedSettingsSlotName = TEXT("GSS_SettingsShared");
};
