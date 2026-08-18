// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CommonUserWidget.h"

#include "GSS_SettingsListEntry.generated.h"

class UGSS_GameSetting;
class UCommonTextBlock;
class UWidget;
struct FFocusEvent;
struct FGeometry;

/**
 * Blueprint base for one GSS setting row in a list view.
 * 列表视图中一个 GSS 设置行的蓝图基类。
 *
 * This intentionally derives from CommonUserWidget instead of a button. A setting row may contain a slider,
 * checkbox, selector or several focusable controls, so projects decide which child controls are interactive.
 * 它有意继承 CommonUserWidget 而非按钮。设置行可能包含滑块、复选框、选择器或多个可聚焦控件，
 * 由项目决定哪些子控件可交互。
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick, Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	/** Returns the runtime setting represented by this row. / 返回此行表示的运行时设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_GameSetting* GetSetting() const { return Setting; }
	/** Returns whether the current setting may accept user input. / 返回当前设置是否可接受用户输入。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	bool IsSettingEnabled() const;
	/** Returns whether the current setting participates in reset-to-default commands. / 返回当前设置是否参与重置为默认值命令。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	bool IsSettingResettable() const;
	/** Returns player-facing reasons that disabled the current setting. / 返回禁用当前设置的面向玩家原因。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<FText> GetDisabledReasons() const;
	/** Returns serialized discrete-option values removed by current edit conditions. / 返回当前编辑条件移除的离散选项序列化值。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<FString> GetDisabledOptionValues() const;
	/** Overrides the setting title for this pooled row without changing the runtime setting. / 仅覆盖此复用行的设置标题，不修改运行时设置。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void SetDisplayNameOverride(const FText& OverrideName);

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	/** Called after a valid runtime setting is assigned. / 有效运行时设置被分配后调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingAssigned(UGSS_GameSetting* AssignedSetting);
	/** Called for pending-value changes and restores. / 待应用值变化或恢复时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingValueChanged();
	/** State-aware variant of OnSettingValueChanged. / 带变更原因的 OnSettingValueChanged 变体。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingValueChangedWithReason(EGSS_GameSettingChangeReason Reason);
	/** Called when visibility, availability or option state changes. / 可见性、可用性或选项状态变化时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingEditableStateChanged();
	/** State-aware variant of OnSettingEditableStateChanged. / 带编辑状态快照的 OnSettingEditableStateChanged 变体。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingEditableStateChangedWithState(FGSS_GameSettingEditableState EditableState);
	/** Returns the child control that should receive gamepad focus when this row is focused. / 本行获得游戏手柄焦点时，应接收焦点的子控件。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	UWidget* GetPrimaryGamepadFocusWidget();

protected:
	/** Refreshes the optional standard setting-title widget. / 刷新可选的标准设置标题 Widget。 */
	void RefreshSettingName();
	virtual void HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason);
	virtual void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting);
	bool CanInteractWithSetting() const;

	/** Suppresses control refresh while a control writes a setting and receives its synchronous change notification. / 控件写入设置并同步收到变更通知时，抑制控件刷新。 */
	bool bSuspendChangeUpdates = false;

	/** Optional standard title for the assigned setting. / 已分配设置的可选标准标题。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonTextBlock> Text_SettingName;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings UI")
	TObjectPtr<UGSS_GameSetting> Setting;

	FText DisplayNameOverride;
};
