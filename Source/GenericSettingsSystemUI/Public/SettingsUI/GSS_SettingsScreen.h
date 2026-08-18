// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Input/UIActionBindingHandle.h"
#include "UI/GUIS_ActivatableWidget.h"
#include "UI/Foundation/GUIS_TabListWidgetBase.h"
#include "SettingsUI/GSS_SettingsActionHandler.h"

#include "GSS_SettingsScreen.generated.h"

class UGSS_SettingsPanel;
class UGSS_SettingsSubsystem;
class UGSS_GameSetting;
class UGSS_GameSettingCollection;
class UCommonButtonBase;
class UWidget;
enum class EGSS_GameSettingChangeReason : uint8;

/**
 * Activatable settings screen that automatically presents its owning LocalPlayer's GSS subsystem.
 * 自动展示所属 LocalPlayer 的 GSS 子系统的可激活设置界面。
 */
UCLASS(Abstract, Blueprintable, meta = (Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsScreen : public UGUIS_ActivatableWidget, public IGSS_SettingsActionHandler
{
	GENERATED_BODY()

public:
	/** Overrides the automatically resolved subsystem, useful for previews and custom player routing. / 覆盖自动解析的子系统，适用于预览和自定义玩家路由。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void SetSettingsSubsystem(UGSS_SettingsSubsystem* InSubsystem);

	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_SettingsSubsystem* GetSettingsSubsystem() const { return SettingsSubsystem; }

	/** Applies pending edits and updates the screen's dirty action state. / 应用待提交修改，并更新界面的脏状态 Action。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void ApplyChanges();
	/** Cancels pending edits and updates the screen's dirty action state. / 取消待提交修改，并更新界面的脏状态 Action。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void CancelChanges();
	/** Returns whether the owning LocalPlayer has unapplied edits. / 返回所属 LocalPlayer 是否有待应用修改。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	bool HavePendingChanges() const;
	/** Handles Back inside the settings hierarchy before allowing GUIS to close this screen. / 在允许 GUIS 关闭本界面前，优先处理设置层级内的返回。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool AttemptToPopNavigation();
	/** Finds a collection and reports whether it has any visible child settings. / 查找 Collection，并报告其是否含有可见子设置。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	UGSS_GameSettingCollection* GetSettingCollection(FGameplayTag SettingId, bool& bHasAnySettings) const;
	/** Restricts the panel to one setting or collection. / 将 Panel 限制为一个设置或 Collection。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void NavigateToSetting(FGameplayTag SettingId);
	/**
	 * Resolves a registered Gameplay Tag from a setting name, then restricts the panel to that setting or Collection.
	 * The request fails and this function returns false when SettingName is None or does not identify a currently registered setting Gameplay Tag.
	 * 根据设置名称解析已注册的 Gameplay Tag，再将 Panel 限制为该设置或 Collection。
	 * 当 SettingName 为 None，或未对应当前已注册设置的 Gameplay Tag 时，请求失败并返回 false。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI", meta = (DisplayName = "Navigate To Setting (Name)"))
	bool NavigateToSettingByName(FName SettingName);
	/** Restricts the panel to the supplied setting and collection roots. / 将 Panel 限制为提供的设置和 Collection 根节点。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void NavigateToSettings(const TArray<FGameplayTag>& SettingIds);
	/**
	 * Recreates automatically managed top-level setting tabs from the current Registry.
	 * This returns false when automatic top tabs are disabled, a tab button class is not configured, or no Registry is available.
	 * 根据当前 Registry 重新创建自动管理的顶层设置 Tab。
	 * 当自动顶层 Tab 被禁用、未配置 Tab 按钮类或 Registry 不可用时返回 false。
	 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool RebuildTopSettingsTabs();
	/** Called whenever pending-change state changes, for example to show Apply and Cancel actions. / 待提交状态变化时调用，例如用于显示应用和取消 Action。 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings UI")
	void OnSettingsDirtyStateChanged(bool bSettingsDirty);

	virtual void OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
	{
	}

	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual bool NativeOnHandleBackAction() override;

protected:
	UFUNCTION()
	void HandleSettingAction(FGameplayTag SettingId, UGSS_GameSetting* Setting);
	UFUNCTION()
	void HandleSettingChanged(FGameplayTag SettingId, UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);
	/** Refreshes dirty Actions after a Registry commit initiated outside this Screen. / 当本 Screen 外部提交 Registry 修改后刷新脏状态 Action。 */
	UFUNCTION()
	void HandleSettingApplied(FGameplayTag SettingId, UGSS_GameSetting* Setting);
	void HandleBackAction();
	void HandleApplyAction();
	void HandleCancelChangesAction();
	UFUNCTION()
	void HandleTopSettingsTabSelected(FName TabId);
	void UpdateDirtyState();
	void BindSubsystemEvents();
	void UnbindSubsystemEvents();
	void BindTopSettingsTabs();
	void UnbindTopSettingsTabs();
	void ClearAutoGeneratedTopSettingsTabs();

	/** Optional top-level settings tab list; tab setup remains Blueprint-owned. / 可选的顶部设置 Tab 列表；Tab 配置仍由蓝图拥有。 */
	UPROPERTY(BlueprintReadOnly, Category = "Input", meta = (BindWidgetOptional, AllowPrivateAccess = true))
	TObjectPtr<UGUIS_TabListWidgetBase> TopSettingsTabs;
	/** Whether this Screen builds tabs from top-level GSS Collections and navigates when one is selected. / 是否由本 Screen 根据顶层 GSS Collection 构建 Tab，并在选中时导航。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GSS|Settings UI")
	bool bAutoBuildTopSettingsTabs = false;
	/** Button class used by automatically created top-level tabs; it must implement GUIS_TabButtonInterface. / 自动创建顶层 Tab 使用的按钮类；必须实现 GUIS_TabButtonInterface。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GSS|Settings UI", meta = (MustImplement = "/Script/GenericUISystem.GUIS_TabButtonInterface", AllowAbstract = "false", EditCondition = "bAutoBuildTopSettingsTabs"))
	TSoftClassPtr<UCommonButtonBase> TopSettingsTabButtonType;
	/** Optional CommonUI action used to return from nested pages or close this screen. / 用于返回嵌套页面或关闭本界面的可选 CommonUI Action。 */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackInputActionData;
	/** Whether root-level Back applies pending changes before closing. Projects can disable this to show a confirmation or discard prompt. / 根级返回是否在关闭前应用待提交修改；项目可关闭它以显示确认或丢弃提示。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GSS|Settings UI",meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	bool bApplyChangesOnBack = false;
	/** Action shown only while there are pending edits. / 仅在存在待应用修改时显示的 Action。 */
	UPROPERTY(EditDefaultsOnly, Category = "Input",meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ApplyInputActionData;
	/** Action shown only while there are pending edits. / 仅在存在待应用修改时显示的 Action。 */
	UPROPERTY(EditDefaultsOnly, Category = "Input",meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle CancelChangesInputActionData;

	FUIActionBindingHandle BackHandle;
	FUIActionBindingHandle ApplyHandle;
	FUIActionBindingHandle CancelChangesHandle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UGSS_SettingsPanel> Settings_Panel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings UI")
	TObjectPtr<UGSS_SettingsSubsystem> SettingsSubsystem;

	/** Tab IDs created by automatic top-level Collection handling. / 自动顶层 Collection 处理所创建的 Tab ID。 */
	UPROPERTY(Transient)
	TArray<FName> AutoGeneratedTopSettingsTabIds;
};
