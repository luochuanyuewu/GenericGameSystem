// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingAction)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingAction
//--------------------------------------

UGSS_GameSettingAction::UGSS_GameSettingAction()
{

}

void UGSS_GameSettingAction::OnInitialized()
{
	Super::OnInitialized();

#if !UE_BUILD_SHIPPING
	ensureMsgf(HasCustomAction() || NamedAction.IsValid(), TEXT("Action settings need either a custom action or a named action."));
	ensureMsgf(!ActionText.IsEmpty(), TEXT("You must provide a ActionText for settings with actions."));
	ensureMsgf(!DescriptionRichText.IsEmpty(), TEXT("You must provide a description for settings with actions."));
#endif
}

void UGSS_GameSettingAction::SetCustomAction(TFunction<void(ULocalPlayer*)> InAction)
{
	CustomAction = UGSS_GameSettingCustomAction::CreateLambda([InAction](UGSS_GameSetting* /*Setting*/, ULocalPlayer* InLocalPlayer) {
		InAction(InLocalPlayer);
	});
}

void UGSS_GameSettingAction::ExecuteAction()
{
	if (HasCustomAction())
	{
		CustomAction.ExecuteIfBound(this, LocalPlayer);
	}
	else
	{
		OnExecuteNamedActionEvent.Broadcast(this, NamedAction);
	}
	
	if (bDirtyAction)
	{
		NotifySettingChanged(EGSS_GameSettingChangeReason::Change);
	}
}

#undef LOCTEXT_NAMESPACE

