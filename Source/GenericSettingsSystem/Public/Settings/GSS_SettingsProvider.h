// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GSS_SettingsDefinition.h"
#include "GSS_SettingsProvider.generated.h"

class UGSS_GameSetting;
class UGSS_GameSettingRegistry;
class UGSS_SettingsSubsystem;
class UGSS_SettingDefinition;

/**
 * Builds runtime settings from the same Definition objects used by Data Assets.
 * 使用与 Data Asset 相同的 Definition 对象构建运行时设置。
 *
 * Builders are supplied only while a provider is registering; do not cache them for later use.
 * Builder 仅在 Provider 注册期间有效，不应缓存后续使用。
 */
UCLASS(BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_SettingsBuilder : public UObject
{
	GENERATED_BODY()

public:
	/** Internal setup performed by the subsystem before registration. / 子系统在注册前执行的内部初始化。 */
	void Initialize(UGSS_SettingsSubsystem* InSubsystem, UGSS_GameSettingRegistry* InRegistry);
	/** Low-level Definition entry shared by Data Assets and C++ providers. Blueprint providers should use the typed Add* nodes. / Data Asset 与 C++ Provider 共用的底层 Definition 入口；蓝图 Provider 应使用类型化 Add* 节点。 */
	UGSS_GameSetting* AddDefinition(const UGSS_SettingDefinition* Definition, UGSS_GameSetting* Parent = nullptr);
	/**
	 * Registers a purpose-built runtime node with this Provider's ownership and lifecycle tracking.
	 * 使用本 Provider 的所有权与生命周期跟踪注册专用运行时节点。
	 *
	 * Use this only when a setting needs behavior beyond the standard Builder node types, such as runtime display
	 * enumeration or an external-device integration. The caller must configure a valid, globally unique SettingId,
	 * developer name and display name before registration.
	 * 仅当设置需要超出标准 Builder 节点类型的行为时使用，例如运行时显示器枚举或外部设备集成。
	 * 调用方必须在注册前配置有效且全局唯一的 SettingId、开发者名称与显示名称。
	 */
	UGSS_GameSetting* AddRuntimeSetting(UGSS_GameSetting* Setting, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddPage(FGameplayTag SettingId, FText DisplayName, FText Description, FText NavigationText, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddCollection(FGameplayTag SettingId, FText DisplayName, FText Description, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddBool(FGameplayTag SettingId, FText DisplayName, FText Description, bool DefaultValue, const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddDiscrete(FGameplayTag SettingId, FText DisplayName, FText Description, FString DefaultValue, const TArray<FGSS_DiscreteOptionDefinition>& Options,
	                              const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddScalar(FGameplayTag SettingId, FText DisplayName, FText Description, double DefaultValue, double MinimumValue, double MaximumValue, double Step,
	                            const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent = nullptr);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	UGSS_GameSetting* AddAction(FGameplayTag SettingId, FText DisplayName, FText Description, FText ActionText, FGameplayTag ActionId, UGSS_GameSetting* Parent = nullptr);
	/** Returns nodes created during this registration, mainly for subsystem ownership and unregistration. / 返回本次注册创建的节点，主要供子系统持有与注销。 */
	const TArray<TObjectPtr<UGSS_GameSetting>>& GetCreatedSettings() const { return CreatedSettings; }
	/** Returns only registration roots, avoiding duplicate recursive unregistration of their children. / 仅返回注册根节点，避免递归注销时重复处理子节点。 */
	TArray<UGSS_GameSetting*> GetCreatedRootSettings() const;

private:
	static void InitializeDefinition(UGSS_SettingDefinition* Definition, FGameplayTag SettingId, FText DisplayName, FText Description, const FGSS_SettingValueAccessor& Accessor);

	UPROPERTY(Transient)
	TObjectPtr<UGSS_SettingsSubsystem> Subsystem;
	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingRegistry> Registry;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> CreatedSettings;
};

/**
 * Unified C++ and Blueprint extension point for conditional or Game Feature settings.
 * 用于条件设置或 Game Feature 设置的统一 C++ / 蓝图扩展点。
 *
 * Register only through the provided Builder so IDs, lifetime tracking, Apply/Cancel and persistence remain shared.
 * 只能通过传入的 Builder 注册，以共享 ID 校验、生命周期跟踪、Apply/Cancel 与持久化流程。
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GENERICSETTINGSSYSTEM_API UGSS_SettingsProvider : public UObject
{
	GENERATED_BODY()

public:
	/** Creates this provider's settings. Called synchronously by RegisterSettingsProvider. / 创建该 Provider 的设置；由 RegisterSettingsProvider 同步调用。 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings")
	void RegisterSettings(UGSS_SettingsBuilder* Builder);
};
