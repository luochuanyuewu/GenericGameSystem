// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"

#include "GSS_GameSettingAction.generated.h"

//--------------------------------------
// UGSS_GameSettingAction
//--------------------------------------

class ULocalPlayer;

DECLARE_DELEGATE_TwoParams(UGSS_GameSettingCustomAction, UGSS_GameSetting* /*Setting*/, ULocalPlayer* /*LocalPlayer*/)

/**
 * 
 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingAction : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingAction();

public:

	DECLARE_EVENT_TwoParams(UGSS_GameSettingAction, FOnExecuteNamedAction, UGSS_GameSetting* /*Setting*/, FGameplayTag /*GameSettings_Action_Tag*/);
	FOnExecuteNamedAction OnExecuteNamedActionEvent;

public:

	FText GetActionText() const { return ActionText; }
	void SetActionText(FText Value) { ActionText = Value; }
#if !UE_BUILD_SHIPPING
	void SetActionText(const FString& Value) { SetActionText(FText::FromString(Value)); }
#endif

	FGameplayTag GetNamedAction() const { return NamedAction; }
	void SetNamedAction(FGameplayTag Value) { NamedAction = Value; }

	bool HasCustomAction() const { return CustomAction.IsBound(); }
	void SetCustomAction(UGSS_GameSettingCustomAction InAction) { CustomAction = InAction; }
	void SetCustomAction(TFunction<void(ULocalPlayer*)> InAction);

	/**
	 * By default actions don't dirty the settings, since the majority of them either do things you can't
	 * revert, or they're things like show the credits or Eula.  However if that's not true, set this flag
	 * to true to fire the change event when this action is fired.
	 */
	void SetDoesActionDirtySettings(bool Value) { bDirtyAction = Value; }

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
