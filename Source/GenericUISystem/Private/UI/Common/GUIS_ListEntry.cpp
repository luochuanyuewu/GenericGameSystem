// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/Common/GUIS_ListEntry.h"

#include "GUIS_LogChannels.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/Actions/GUIS_UIAction.h"

void UGUIS_ListEntry::Bp_RegisterUIActionBinding(FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback,
                                                 FGUIS_UIActionBindingHandle& BindingHandle)
{
	FBindUIActionArgs BindArgs(InputAction, bShouldDisplayInActionBar, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		Callback.ExecuteIfBound(InputAction.RowName);
	}));

	BindingHandle.Handle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle.Handle);
}

void UGUIS_ListEntry::Bp_RegisterEntryActionBinding(UGUIS_UIAction* EntryAction, bool bShouldDisplayInActionBar, const FGUIS_UIEntryActionExecutedDelegate& Callback,
                                                   FGUIS_UIActionBindingHandle& BindingHandle)
{
	if (EntryAction == nullptr)
		return;


	FBindUIActionArgs BindArgs(EntryAction->GetInputActionData(), bShouldDisplayInActionBar, FSimpleDelegate::CreateLambda([Callback,EntryAction]()
	{
		Callback.ExecuteIfBound(EntryAction);
	}));

	BindingHandle.Handle = RegisterUIActionBinding(BindArgs);
	BindingHandle.Id = EntryAction->GetActionID();
	BindingHandles.Add(BindingHandle.Handle);
	UE_LOG(LogGUIS, Display, TEXT("Register binding for %s"), *BindingHandle.Handle.GetDisplayName().ToString())
}

void UGUIS_ListEntry::UnregisterBinding(FGUIS_UIActionBindingHandle& BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		UE_LOG(LogGUIS, Display, TEXT("Unregister binding for %s"), *BindingHandle.Handle.GetDisplayName().ToString())

		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
	}
}

void UGUIS_ListEntry::UnregisterAllBindings()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		Handle.Unregister();
	}
	BindingHandles.Empty();
}

void UGUIS_ListEntry::NativeDestruct()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		if (Handle.IsValid())
		{
			Handle.Unregister();
		}
	}
	BindingHandles.Empty();
	Super::NativeDestruct();
}

// void UGUIS_ListEntry::NativeOnItemSelectionChanged(bool bIsSelected)
// {
// 	if (bIsSelected && !EntryActionsOnSelected.IsEmpty())
// 	{
// 		for (int32 i = 0; i < EntryActionsOnSelected.Num(); i++)
// 		{
// 			if (!IsValid(EntryActionsOnSelected[i]))
// 				continue;
// 			UGUIS_EntryAction* EntryAction = EntryActionsOnSelected[i];
//
// 			FBindUIActionArgs BindArgs(EntryAction->GetInputActionData(), true, FSimpleDelegate::CreateLambda([this,EntryAction]()
// 			{
// 				HandleEntryAction(EntryAction);
// 			}));
// 			RegisterUIActionBinding()
// 		}
// 	}
// 	else
// 	{
// 	}
// 	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
// }
