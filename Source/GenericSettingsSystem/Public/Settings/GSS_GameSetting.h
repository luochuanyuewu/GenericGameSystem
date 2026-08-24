// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Components/SlateWrapperTypes.h"
#include "GSS_GameSettingFilterState.h"
#include "GSS_GameSettingEditCondition.h"
#include "UObject/WeakObjectPtr.h"

#include "GSS_GameSetting.generated.h"

class ULocalPlayer;
class UGSS_GameSettingRegistry;

//--------------------------------------
// UGSS_GameSetting
//--------------------------------------

/** Produces contextual, non-searchable details for a setting. / 为设置生成上下文相关且不可搜索的详情文本。 */
DECLARE_DELEGATE_RetVal_OneParam(FText, FGetGameSettingsDetails, ULocalPlayer& /*InLocalPlayer*/);

/**
 * Base runtime node in a GSS settings tree.
 * GSS 设置树中的运行时节点基类。
 *
 * Settings keep a current editable state and participate in Registry change events. Value subclasses add
 * pending/apply/restore semantics; collection and action subclasses provide hierarchy and commands.
 * 设置保存当前可编辑状态并参与 Registry 变更事件。Value 子类提供待应用/提交/恢复语义；
 * Collection 与 Action 子类分别提供层级与命令。
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSetting : public UObject
{
	GENERATED_BODY()

public:
	UGSS_GameSetting()
	{
	}

public:
	/** Native event emitted when this node's editable value or dependency state changes. / 节点的可编辑值或依赖状态变化时发出的原生事件。 */
	DECLARE_EVENT_TwoParams(UGSS_GameSetting, FOnSettingChanged, UGSS_GameSetting* /*InSetting*/, EGSS_GameSettingChangeReason /*InChangeReason*/);

	/** Native event emitted after a successful Apply. / 成功 Apply 后发出的原生事件。 */
	DECLARE_EVENT_OneParam(UGSS_GameSetting, FOnSettingApplied, UGSS_GameSetting* /*InSetting*/);

	/** Native event emitted after visibility or enabled state is recomputed. / 可见性或可用状态重新计算后发出的原生事件。 */
	DECLARE_EVENT_OneParam(UGSS_GameSetting, FOnSettingEditConditionChanged, UGSS_GameSetting* /*InSetting*/);

	FOnSettingChanged OnSettingChangedEvent;
	FOnSettingApplied OnSettingAppliedEvent;
	FOnSettingEditConditionChanged OnSettingEditConditionChangedEvent;

public:
	/**
	 * Gets the non-localized developer name, unique within this Registry.
	 * 获取非本地化的开发者名称；它在当前 Registry 内必须唯一。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FName GetDevName() const { return DevName; }

	/** Returns the global stable ID used by queries and navigation. / 返回查询与导航所用的全局稳定 ID。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	FGameplayTag GetSettingId() const { return SettingId; }

	void SetSettingId(const FGameplayTag& Value) { SettingId = Value; }

	void SetDevName(const FName& Value) { DevName = Value; }

	bool GetAdjustListViewPostRefresh() const { return bAdjustListViewPostRefresh; }
	void SetAdjustListViewPostRefresh(const bool Value) { bAdjustListViewPostRefresh = Value; }

	/** Returns the localized UI title. / 返回本地化 UI 标题。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FText GetDisplayName() const { return DisplayName; }

	void SetDisplayName(const FText& Value) { DisplayName = Value; }
#if !UE_BUILD_SHIPPING
	void SetDisplayName(const FString& Value) { SetDisplayName(FText::FromString(Value)); }
#endif
	/** Returns how the title should be displayed. / 返回标题应如何显示。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	ESlateVisibility GetDisplayNameVisibility() { return DisplayNameVisibility; }

	void SetNameDisplayVisibility(ESlateVisibility InVisibility) { DisplayNameVisibility = InVisibility; }

	/** Returns the localized rich-text description. / 返回本地化富文本说明。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FText GetDescriptionRichText() const { return DescriptionRichText; }

	void SetDescriptionRichText(const FText& Value)
	{
		DescriptionRichText = Value;
		InvalidateSearchableText();
	}

#if !UE_BUILD_SHIPPING
	/** Development-only convenience overload for non-localized text. / 仅开发版本可用的非本地化文本便捷重载。 */
	void SetDescriptionRichText(const FString& Value) { SetDescriptionRichText(FText::FromString(Value)); }
#endif

	/** Returns arbitrary tags used by callers or UI filtering. / 返回供调用方或 UI 筛选使用的任意标签。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	const FGameplayTagContainer& GetTags() const { return Tags; }

	/** Adds an arbitrary metadata tag. / 添加任意元数据标签。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void AddTag(const FGameplayTag& TagToAdd) { Tags.AddTag(TagToAdd); }

	/** Sets the owning Registry; normally performed during registration. / 设置所属 Registry；通常在注册期间完成。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void SetRegistry(UGSS_GameSettingRegistry* InOwningRegistry) { OwningRegistry = InOwningRegistry; }

	/** Returns the owning Registry after registration. / 返回注册后所属的 Registry。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	UGSS_GameSettingRegistry* GetRegistry() const { return OwningRegistry; }

	/** Gets a searchable plain-text form of the description. / 获取说明文本的可搜索纯文本形式。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	const FString& GetDescriptionPlainText() const;

	/** Initializes the node for its owning LocalPlayer; initialized collections initialize added children. / 为所属 LocalPlayer 初始化节点；已初始化的 Collection 会初始化新添加的子节点。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void Initialize(ULocalPlayer* InLocalPlayer);

	/** Gets the owning LocalPlayer after initialization. / 获取初始化后的所属 LocalPlayer。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	ULocalPlayer* GetOwningLocalPlayer() const { return LocalPlayer; }

	/** Sets a callback for contextual description-panel details; its text is not searchable. / 设置说明面板的上下文详情回调；返回文本不可搜索。 */
	void SetDynamicDetails(const FGetGameSettingsDetails& InDynamicDetails) { DynamicDetails = InDynamicDetails; }

	/**
	 * Gets contextual details such as account-specific status. / 获取账户状态等上下文详情。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FText GetDynamicDetails() const;

	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	FText GetWarningRichText() const { return WarningRichText; }

	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void SetWarningRichText(const FText& Value)
	{
		WarningRichText = Value;
		InvalidateSearchableText();
	}

#if !UE_BUILD_SHIPPING
	/** Development-only convenience overload for non-localized warning text. / 仅开发版本可用的非本地化警告文本便捷重载。 */
	void SetWarningRichText(const FString& Value) { SetWarningRichText(FText::FromString(Value)); }
#endif

	/**
	 * Gets the cached native editable state computed from edit conditions.
	 * 获取由编辑条件计算的缓存原生编辑状态。
	 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings")
	const FGSS_GameSettingEditableState& GetEditState() const { return EditableStateCache; }

	/**
	 * Adds an edit condition that controls this node's visibility, availability or options.
	 * Safe before or after Initialize: a live node initializes the condition and recomputes EditableState immediately.
	 * 添加控制节点可见性、可用性或选项的编辑条件。
	 * 在 Initialize 前后都可以调用；节点已初始化时会立刻初始化该条件并重算 EditableState。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void AddEditCondition(UGSS_GameSettingEditCondition* InEditCondition);

	/**
	 * Adds a dependency by SettingId. Changes to that setting refresh this node's edit state.
	 * The other setting may be registered later; the Registry resolves the tag when it appears.
	 * 按 SettingId 添加依赖；该设置变化时会刷新本节点的编辑状态。
	 * 对方可以稍后注册，Registry 会在其出现时解析该 Tag。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void AddEditDependency(UPARAM(meta = (Categories = "GSS.Settings")) FGameplayTag DependencySettingId);

	/** Adds several SettingId dependencies. / 添加多个 SettingId 依赖。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings")
	void AddEditDependencies(const TArray<FGameplayTag>& DependencySettingIds);

	/** Resolves pending SettingId dependencies against the owning Registry. / 按所属 Registry 解析尚未绑定的 SettingId 依赖。 */
	void ResolveEditDependencies();

	/** Sets the parent setting, normally a collection. / 设置父设置，通常为 Collection。 */
	void SetSettingParent(UGSS_GameSetting* InSettingParent);
	UGSS_GameSetting* GetSettingParent() const { return SettingParent; }

	/**
	 * True once any asynchronous initialization has completed. / 任意异步初始化完成后返回 true。
	 */
	bool IsReady() const { return bReady; }

	/**
	 * Returns child nodes; collections override this to expose their hierarchy.
	 * 返回子节点；Collection 会重写此方法以提供层级。
	 */
	virtual TArray<UGSS_GameSetting*> GetChildSettings() { return TArray<UGSS_GameSetting*>(); }

	/**
	 * Recomputes the editable state and optionally notifies listeners.
	 * 重新计算可编辑状态，并可选地通知监听者。
	 */
	void RefreshEditableState(bool bNotifyEditConditionsChanged = true);

	/** Applies this node's pending change. / 应用此节点的待应用修改。
	 * We expect settings to change the live value immediately, but occasionally there are special settings
	 * that go are immediately stored to a temporary location but we don't actually apply them until later
	 * like selecting a new resolution.
	 */
	void Apply();

	/** Gets the current world of the owning LocalPlayer. / 获取所属 LocalPlayer 当前所在的世界。 */
	virtual UWorld* GetWorld() const override;

protected:
	/** Starts initialization; subclasses may complete asynchronously via StartupComplete. / 开始初始化；子类可通过 StartupComplete 异步完成。 */
	virtual void Startup();
	void StartupComplete();

	virtual void OnInitialized();
	virtual void OnApply();
	virtual void OnGatherEditState(FGSS_GameSettingEditableState& InOutEditState) const;
	virtual void OnDependencyChanged();

	/** Returns subclass-specific contextual detail text. / 返回子类特有的上下文详情文本。 */
	virtual FText GetDynamicDetailsInternal() const;

	/** Handles an edit dependency change. / 处理编辑依赖变化。 */
	void HandleEditDependencyChanged(UGSS_GameSetting* DependencySetting, EGSS_GameSettingChangeReason Reason);
	void HandleEditDependencyChanged(UGSS_GameSetting* DependencySetting);
	void BindEditDependency(FGameplayTag DependencySettingId);

	/** Regenerates cached plain searchable text when invalidated. / 失效后重新生成缓存的可搜索纯文本。 */
	void RefreshPlainText() const;
	void InvalidateSearchableText() { bRefreshPlainSearchableText = true; }

	/** Broadcasts a setting-change notification. / 广播设置变更通知。 */
	void NotifySettingChanged(EGSS_GameSettingChangeReason Reason);
	virtual void OnSettingChanged(EGSS_GameSettingChangeReason Reason);

	/** Broadcasts an edit-condition change such as visibility, enabled state or option availability. / 广播编辑条件变化，例如可见性、可用性或选项可用性。 */
	void NotifyEditConditionsChanged();
	virtual void OnEditConditionsChanged();

	/** Computes the current editable state from every condition. / 根据全部条件计算当前可编辑状态。 */
	void ComputeEditableState();

protected:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSetting> SettingParent;

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingRegistry> OwningRegistry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FName DevName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FGameplayTag SettingId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	ESlateVisibility DisplayNameVisibility = ESlateVisibility::SelfHitTestInvisible;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText DescriptionRichText;
	FText WarningRichText;

	/** A collection of tags for the settings.  These can just be arbitrary flags used by the UI to do different things. */
	FGameplayTagContainer Tags;

	FGetGameSettingsDetails DynamicDetails;

	/** Runtime edit conditions for this setting. / 此设置的运行时编辑条件。 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSettingEditCondition>> EditConditions;

	/** SettingIds whose changes refresh this node. / 变化时会刷新本节点的 SettingId。 */
	UPROPERTY(Transient)
	TArray<FGameplayTag> EditDependencyIds;

	TMap<FGameplayTag, TWeakObjectPtr<UGSS_GameSetting>> BoundEditDependencies;

	class FStringCultureCache
	{
		FStringCultureCache(TFunction<FString()> InStringGetter);

		void Invalidate();

		FString Get() const;

	private:
		mutable FString StringCache;
		mutable FCultureRef Culture;
		TFunction<FString()> StringGetter;
	};

	/** When the text changes, we invalidate the searchable text. */
	mutable bool bRefreshPlainSearchableText = true;
	/** When we set the rich text for a setting, we automatically generate the plain text. */
	mutable FString AutoGenerated_DescriptionPlainText;

private:
	/** Most settings are immediately ready, but some may require startup time before it's safe to call their functions. */
	bool bReady = false;

	/** Prevent re-entrancy problems when announcing a setting has changed. */
	bool bOnSettingChangedEventGuard = false;

	/** Prevent re-entrancy problems when announcing a setting has changed edit conditions. */
	bool bOnEditConditionsChangedEventGuard = false;

	/**  */
	bool bAdjustListViewPostRefresh = true;

	/** Cached editable state exposed to UI and Blueprint. / 暴露给 UI 与蓝图的缓存编辑状态。 */
	FGSS_GameSettingEditableState EditableStateCache;
};
