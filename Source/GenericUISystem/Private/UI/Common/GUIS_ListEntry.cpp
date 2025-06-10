// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/Common/GUIS_ListEntry.h"
#include "UI/GUIS_GameUISubsystem.h"

void UGUIS_ListEntry::Bp_RegisterUIActionBinding(FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback,
                                                 FGUIS_UIActionBindingHandle& BindingHandle)
{
}

void UGUIS_ListEntry::Bp_RegisterEntryActionBinding(UGUIS_UIAction* EntryAction, bool bShouldDisplayInActionBar, const FGUIS_UIEntryActionExecutedDelegate& Callback,
                                                    FGUIS_UIActionBindingHandle& BindingHandle)
{
}

void UGUIS_ListEntry::UnregisterBinding(FGUIS_UIActionBindingHandle& BindingHandle)
{
}

void UGUIS_ListEntry::UnregisterAllBindings()
{
}
