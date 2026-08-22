// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GSS_GameSettingFilterState.h"
#include "GSS_GameSettingsSubsystem.generated.h"

class UGSS_GameSetting;
class UGSS_GameSettingRegistry;
class UGSS_GameSettingsDefinitionAsset;
class UGSS_GameSettingsProvider;
class UGSS_SettingsShared;
class UGSS_GameSettingsBuilder;
class FGSS_GameSettingRegistryChangeTracker;

/**
 * Opaque token returned when a runtime provider is registered.
 * 运行时 Provider 注册成功后返回的不透明令牌；请保存它以便在 Provider 停用时注销。
 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEM_API FGSS_SettingsRegistrationHandle
{
	GENERATED_BODY()

	/** Returns whether this token was produced by a successful registration. / 返回该令牌是否来自成功注册。 */
	bool IsValid() const { return Id.IsValid(); }

private:
	friend class UGSS_GameSettingsSubsystem;
	FGuid Id;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGSS_OnSettingPendingChanged, FGameplayTag, SettingId, UGSS_GameSetting*, Setting, EGSS_GameSettingChangeReason, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSS_OnSettingNodeEvent, FGameplayTag, SettingId, UGSS_GameSetting*, Setting);

/**
 * Per-local-player entry point for the GSS settings runtime.
 * 每个本地玩家各自拥有的 GSS 设置运行时入口。
 *
 * The subsystem builds the configured Definition Asset, owns the Registry and dirty-change tracker,
 * and persists the player's preferences. Dynamic settings write through their Accessors immediately;
 * ApplyChanges persists Local engine settings and Shared Settings.
 * 子系统构建配置的 Definition Asset、持有 Registry 与脏值跟踪器，并持久化该玩家的偏好。
 * Dynamic 设置会立即通过 Accessor 写入；ApplyChanges 再持久化本地引擎设置与 Shared Settings。
 */
UCLASS(BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingsSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Returns the subsystem for the context's LocalPlayer; world contexts resolve to their first local player. / 返回上下文所属 LocalPlayer 的子系统；World 上下文使用其第一个本地玩家。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings", meta = (WorldContext = "WorldContextObject"))
	static UGSS_GameSettingsSubsystem* Get(const UObject* WorldContextObject);

	/** Returns this player's runtime settings registry. / 返回该玩家的运行时设置注册表。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_GameSettingRegistry* GetRegistry() const { return Registry; }

	/** Reads the current boolean value identified by SettingId. / 按 SettingId 读取当前布尔值。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	bool GetBoolSettingValue(FGameplayTag SettingId, bool DefaultValue = false) const;

	/** Writes a boolean Dynamic setting immediately; call ApplyChanges to persist it. / 立即写入布尔 Dynamic 设置；调用 ApplyChanges 后才会持久化。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	bool SetBoolSettingValue(FGameplayTag SettingId, bool Value);
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") FString GetStringSettingValue(FGameplayTag SettingId, const FString& DefaultValue = TEXT("")) const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") bool SetStringSettingValue(FGameplayTag SettingId, const FString& Value);
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") double GetScalarSettingValue(FGameplayTag SettingId, double DefaultValue = 0.0) const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") bool SetScalarSettingValue(FGameplayTag SettingId, double Value);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") bool ExecuteAction(FGameplayTag SettingId);

	/** Returns this LocalPlayer's portable Shared Settings object. / 返回此 LocalPlayer 的可携带 Shared Settings 对象。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_SettingsShared* GetSharedSettings() const { return SharedSettings; }

	/** Applies remaining staged nodes, then persists local engine settings and Shared Settings. / 应用仍需提交的节点，随后持久化本地引擎设置与 Shared Settings。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void ApplyChanges();
	/** Returns whether this LocalPlayer has unapplied setting edits. / 返回此 LocalPlayer 是否存在尚未应用的设置修改。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	bool HavePendingChanges() const;

	/** Discards all pending edits and restores each affected setting to its last applied value. / 丢弃全部待应用修改，并恢复为最后一次已应用值。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void CancelChanges();

	/** Reloads Shared Settings, applies stored preferences, and refreshes registered values from their Accessors. Pending edits are discarded. / 重新加载 Shared Settings、应用已存偏好，并从 Accessor 刷新已注册值；待应用修改会被丢弃。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void Reload();

	/** Invokes Provider registration and returns a token for later removal of its complete setting subtree. / 调用 Provider 注册，并返回用于移除其完整设置子树的令牌。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FGSS_SettingsRegistrationHandle RegisterSettingsProvider(UGSS_GameSettingsProvider* Provider);

	/** Removes every setting created by the registration token and restores then discards their pending edits. / 移除该令牌创建的全部设置，先恢复再丢弃它们的待应用修改。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void UnregisterSettingsProvider(FGSS_SettingsRegistrationHandle Handle);

	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingPendingChanged OnSettingPendingChanged;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingApplied;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingActionExecuted;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingNavigationRequested;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingEditStateChanged;

private:
	void BuildDefinitions(UGSS_GameSettingsDefinitionAsset* DefinitionAsset);
	void LoadSharedSettings();
	void SaveChanges();
	void HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);
	void HandleSettingApplied(UGSS_GameSetting* Setting);
	void HandleSettingAction(UGSS_GameSetting* Setting, FGameplayTag ActionId);
	void HandleSettingNavigation(UGSS_GameSetting* Setting);
	void HandleSettingEditState(UGSS_GameSetting* Setting);

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingRegistry> Registry;

	UPROPERTY(Transient)
	TObjectPtr<UGSS_SettingsShared> SharedSettings;

	TMap<FGuid, TArray<TObjectPtr<UGSS_GameSetting>>> ProviderSettings;
	/** Keeps automatically created providers alive for this LocalPlayer. / 保持为此 LocalPlayer 自动创建的 Provider 存活。 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSettingsProvider>> StartupProviders;

	TUniquePtr<FGSS_GameSettingRegistryChangeTracker> ChangeTracker;
};
