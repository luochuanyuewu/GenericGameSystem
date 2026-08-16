// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CommonUserWidget.h"

#include "GSS_SettingsListEntry.generated.h"

class UGSS_GameSetting;
class UCommonTextBlock;

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

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;

	/** Called after a valid runtime setting is assigned. / 有效运行时设置被分配后调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingAssigned(UGSS_GameSetting* AssignedSetting);
	/** Called for pending-value changes and restores. / 待应用值变化或恢复时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingValueChanged();
	/** Called when visibility, availability or option state changes. / 可见性、可用性或选项状态变化时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingEditableStateChanged();

protected:
	/** Refreshes the optional standard setting-title widget. / 刷新可选的标准设置标题 Widget。 */
	void RefreshSettingName();
	void HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason);
	void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting);

	/** Optional standard title for the assigned setting. / 已分配设置的可选标准标题。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonTextBlock> Text_SettingName;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings UI")
	TObjectPtr<UGSS_GameSetting> Setting;
};
