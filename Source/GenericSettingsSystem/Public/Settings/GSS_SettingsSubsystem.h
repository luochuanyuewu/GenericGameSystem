// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"
#include "GSS_GameSettingFilterState.h"
#include "GSS_SettingsSubsystem.generated.h"

class UGSS_GameSetting;
class UGSS_GameSettingRegistry;
class UGSS_SettingsDefinitionAsset;
class UGSS_SettingsProvider;
class UGSS_SettingsShared;
class UGSS_SettingsBuilder;
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
	friend class UGSS_SettingsSubsystem;
	FGuid Id;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGSS_OnSettingPendingChanged, FGameplayTag, SettingId, UGSS_GameSetting*, Setting, EGSS_GameSettingChangeReason, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSS_OnSettingNodeEvent, FGameplayTag, SettingId, UGSS_GameSetting*, Setting);

/**
 * Per-local-player entry point for the GSS settings runtime.
 * 每个本地玩家各自拥有的 GSS 设置运行时入口。
 *
 * The subsystem builds the configured Definition Asset, owns the Registry and pending-change tracker,
 * and persists the player's applied preferences. Use Set* methods for previewable edits, then explicitly
 * call ApplyChanges or CancelChanges.
 * 子系统构建配置的 Definition Asset、持有 Registry 与待应用变更跟踪器，并持久化该玩家已应用的偏好。
 * 使用 Set* 方法修改可预览的待应用值，随后显式调用 ApplyChanges 或 CancelChanges。
 */
UCLASS(BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_SettingsSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Returns the subsystem for the context's LocalPlayer; world contexts resolve to their first local player. / 返回上下文所属 LocalPlayer 的子系统；World 上下文使用其第一个本地玩家。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings", meta = (WorldContext = "WorldContextObject"))
	static UGSS_SettingsSubsystem* Get(const UObject* WorldContextObject);

	/** Returns this player's runtime settings registry. / 返回该玩家的运行时设置注册表。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_GameSettingRegistry* GetRegistry() const { return Registry; }

	/** Reads the effective (including pending, unapplied) boolean value identified by SettingId. / 按 SettingId 读取当前有效值（包含尚未应用的修改）。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	bool GetBoolSettingValue(FGameplayTag SettingId, bool DefaultValue = false) const;

	/** Changes a boolean setting in the pending edit state; call ApplyChanges to persist it. / 修改布尔设置的待应用值；调用 ApplyChanges 后才会持久化。 */
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

	/** Applies pending edits, applies local engine settings and saves Shared Settings. Failed value writes remain pending. / 应用待提交修改、应用本地引擎设置并保存 Shared Settings；写入失败的值保持待提交。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void ApplyChanges();
	/** Returns whether this LocalPlayer has unapplied setting edits. / 返回此 LocalPlayer 是否存在尚未应用的设置修改。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	bool HavePendingChanges() const;

	/** Discards all pending edits and restores each affected setting to its last applied value. / 丢弃全部待应用修改，并恢复为最后一次已应用值。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void CancelChanges();

	/** Saves the current Shared Settings object for this LocalPlayer. / 保存此 LocalPlayer 当前的 Shared Settings 对象。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void Save();

	/** Reloads Shared Settings and refreshes registered values from their Accessors. Pending edits are discarded. / 重新加载 Shared Settings 并从 Accessor 刷新已注册值；待应用修改会被丢弃。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void Reload();

	/** Invokes Provider registration and returns a token for later removal of its complete setting subtree. / 调用 Provider 注册，并返回用于移除其完整设置子树的令牌。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FGSS_SettingsRegistrationHandle RegisterSettingsProvider(UGSS_SettingsProvider* Provider);

	/** Removes every setting created by the registration token and discards their pending edits. / 移除该令牌创建的全部设置，并丢弃它们的待应用修改。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void UnregisterSettingsProvider(FGSS_SettingsRegistrationHandle Handle);

	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingPendingChanged OnSettingPendingChanged;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingApplied;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingActionExecuted;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingNavigationRequested;
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings") FGSS_OnSettingNodeEvent OnSettingEditStateChanged;

private:
	void BuildDefinitions(UGSS_SettingsDefinitionAsset* DefinitionAsset);
	void LoadSharedSettings();
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
	TArray<TObjectPtr<UGSS_SettingsProvider>> StartupProviders;

	TUniquePtr<FGSS_GameSettingRegistryChangeTracker> ChangeTracker;
};
