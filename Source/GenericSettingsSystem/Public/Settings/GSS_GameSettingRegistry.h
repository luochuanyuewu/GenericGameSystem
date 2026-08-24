// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"
#include "Templates/Casts.h"

#include "GSS_GameSettingRegistry.generated.h"

struct FGameplayTag;

//--------------------------------------
// UGSS_GameSettingRegistry
//--------------------------------------

class ULocalPlayer;
struct FGSS_GameSettingFilterState;

enum class EGSS_GameSettingChangeReason : uint8;

/**
 * Runtime owner and query index for one LocalPlayer's setting tree.
 * 一个 LocalPlayer 设置树的运行时所有者与查询索引。
 *
 * The subsystem creates and owns the default registry. C++ extensions may subclass it, but Data Asset and
 * Provider content should be added through the Builder/Register APIs so lifecycle and event aggregation stay correct.
 * 子系统创建并持有默认 Registry。C++ 扩展可以继承它，但 Data Asset 与 Provider 内容应通过 Builder/Register API 添加，
 * 以确保生命周期和事件聚合正确。
 */
UCLASS(Blueprintable, BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingRegistry : public UObject
{
	GENERATED_BODY()

public:
	/** Native event for a pending setting edit or restoration. / 原生事件：待应用设置被修改或恢复。 */
	DECLARE_EVENT_TwoParams(UGSS_GameSettingRegistry, FOnSettingChanged, UGSS_GameSetting*, EGSS_GameSettingChangeReason);

	/** Native event for visibility, enabled-state or option availability changes. / 原生事件：可见性、可用性或选项可用性发生变化。 */
	DECLARE_EVENT_OneParam(UGSS_GameSettingRegistry, FOnSettingEditConditionChanged, UGSS_GameSetting*);

	FOnSettingChanged OnSettingChangedEvent;
	FOnSettingEditConditionChanged OnSettingEditConditionChangedEvent;
	DECLARE_EVENT_OneParam(UGSS_GameSettingRegistry, FOnSettingAppliedEvent, UGSS_GameSetting*);
	FOnSettingAppliedEvent OnSettingAppliedEvent;

	/** Native event forwarded from an executed named action. / 从已执行命名 Action 转发的原生事件。 */
	DECLARE_EVENT_TwoParams(UGSS_GameSettingRegistry, FOnSettingNamedActionEvent, UGSS_GameSetting* /*Setting*/, FGameplayTag /*GameSettings_Action_Tag*/);

	FOnSettingNamedActionEvent OnSettingNamedActionEvent;

	/** Native event requesting navigation to a page's child settings. / 请求导航到页面子设置的原生事件。 */
	DECLARE_EVENT_OneParam(UGSS_GameSettingRegistry, FOnExecuteNavigation, UGSS_GameSetting* /*Setting*/);

	FOnExecuteNavigation OnExecuteNavigationEvent;

public:
	UGSS_GameSettingRegistry();

	/** Initializes this registry for one LocalPlayer. It is normally called by the subsystem. / 为一个 LocalPlayer 初始化 Registry；通常由子系统调用。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void Initialize(ULocalPlayer* InLocalPlayer);

	/** True once every registered setting has completed initialization. / 当所有已注册设置均完成初始化时返回 true。 */
	virtual bool IsFinishedInitializing() const;

	/** Appends settings that pass FilterState, respecting collection/page hierarchy. / 追加通过 FilterState 的设置，并保留 Collection/Page 层级规则。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& OutSettings);

	/** Finds a runtime setting by its developer name. / 按开发者名称查找运行时设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_GameSetting* FindSettingByDevName(const FName& SettingDevName);

	/** Finds a runtime setting by its globally stable Gameplay Tag ID. / 按全局稳定 Gameplay Tag ID 查找运行时设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_GameSetting* FindSettingById(const FGameplayTag& SettingId) const;

	/**
	 * Returns registered top-level inline Collections in their registration order.
	 * Returns no top-level Page nodes because Pages represent in-tree navigation rather than settings-screen categories.
	 * 按注册顺序返回顶层内联 Collection。
	 * 不返回顶层 Page 节点，因为 Page 表示树内导航，而非设置界面的分类。
	 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	TArray<UGSS_GameSettingCollection*> GetTopLevelSettingCollections() const;

	/** Adds a node and its descendants to this registry. Parent may be null for a root node. / 将节点及其后代添加到此 Registry；Parent 为空时作为根节点。 */
	void RegisterSetting(UGSS_GameSetting* InSetting, UGSS_GameSettingCollection* Parent = nullptr);
	/** Removes a node and all descendants, including registry event bindings. / 移除节点及其全部后代，并解除 Registry 事件绑定。 */
	void UnregisterSetting(UGSS_GameSetting* InSetting);
	/** Re-reads applied values through Accessors and notifies runtime listeners. / 通过 Accessor 重读已应用值，并通知运行时监听者。 */
	/** Reloads all value nodes from their Local or Shared Accessors. / 从各值节点的 Local 或 Shared Accessor 重新加载。 */
	void ReloadSettingsFromAccessors();

	template <typename T = UGSS_GameSetting>
	T* FindSettingByDevNameChecked(const FName& SettingDevName)
	{
		T* Setting = Cast<T>(FindSettingByDevName(SettingDevName));
		check(Setting);
		return Setting;
	}

	// Internal registration and event handlers.
	void RegisterSettingTree(UGSS_GameSetting* InSetting);
	void ResolveRegisteredEditDependencies();
	void HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);
	void HandleSettingApplied(UGSS_GameSetting* Setting);
	void HandleSettingEditConditionsChanged(UGSS_GameSetting* Setting);
	void HandleSettingNamedAction(UGSS_GameSetting* Setting, FGameplayTag GameSettings_Action_Tag);
	void HandleSettingNavigation(UGSS_GameSetting* Setting);

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> TopLevelSettings;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> RegisteredSettings;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> OwningLocalPlayer;
};
