// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"

#include "GSS_GameSettingAction.generated.h"

//--------------------------------------
// UGSS_GameSettingAction
//--------------------------------------

class ULocalPlayer;

/** Native callback invoked when an action setting executes. / Action 设置执行时调用的原生回调。 */
DECLARE_DELEGATE_TwoParams(UGSS_GameSettingCustomAction, UGSS_GameSetting* /*Setting*/, ULocalPlayer* /*LocalPlayer*/)

/**
 * A command node displayed in a settings tree rather than a persisted value.
 * 显示在设置树中的命令节点，而非持久化数值。
 *
 * Actions can invoke a local callback and/or emit a Gameplay Tag through the Registry.
 * Action 可调用本地回调，也可通过 Registry 发出 Gameplay Tag。
 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingAction : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingAction();

public:

	/** Native event emitted when the configured named action executes. / 配置的命名 Action 执行时发出的原生事件。 */
	DECLARE_EVENT_TwoParams(UGSS_GameSettingAction, FOnExecuteNamedAction, UGSS_GameSetting* /*Setting*/, FGameplayTag /*GameSettings_Action_Tag*/);
	FOnExecuteNamedAction OnExecuteNamedActionEvent;

public:

	/** Returns the localized label for the command button. / 返回命令按钮的本地化标签。 */
	FText GetActionText() const { return ActionText; }
	void SetActionText(FText Value) { ActionText = Value; }
#if !UE_BUILD_SHIPPING
	void SetActionText(const FString& Value) { SetActionText(FText::FromString(Value)); }
#endif

	/** Returns the tag forwarded to Registry action listeners. / 返回转发给 Registry Action 监听者的标签。 */
	FGameplayTag GetNamedAction() const { return NamedAction; }
	void SetNamedAction(FGameplayTag Value) { NamedAction = Value; }

	/** Returns whether a native callback is bound. / 返回是否绑定了原生回调。 */
	bool HasCustomAction() const { return CustomAction.IsBound(); }
	void SetCustomAction(UGSS_GameSettingCustomAction InAction) { CustomAction = InAction; }
	void SetCustomAction(TFunction<void(ULocalPlayer*)> InAction);

	/** Marks execution as a pending settings change when true; false is appropriate for irreversible commands. / 为 true 时将执行标记为待应用设置修改；不可逆命令通常应保持 false。 */
	void SetDoesActionDirtySettings(bool Value) { bDirtyAction = Value; }

	/** Executes the callback, named action event, and optional dirty notification. / 执行回调、命名 Action 事件以及可选的脏状态通知。 */
	virtual void ExecuteAction();

protected:
	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;

protected:
	FText ActionText;
	FGameplayTag NamedAction;
	UGSS_GameSettingCustomAction CustomAction;
	bool bDirtyAction = false;
};
