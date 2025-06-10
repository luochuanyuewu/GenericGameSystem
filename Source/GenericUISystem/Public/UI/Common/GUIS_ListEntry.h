// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/GUIS_GameUISubsystem.h"
#include "UI/Foundation/GUIS_ButtonBase.h"
#include "GUIS_ListEntry.generated.h"

class UGUIS_UIAction;
DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIActionExecutedDelegate, FName, ActionName);

DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIEntryActionExecutedDelegate, UGUIS_UIAction *, EntryAction);


/**
 * ListEntry(as CommonButton) will represent a list item(Item of ListView/TileView)
 */
UCLASS(Abstract, meta = (Category = "Generic UI", DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_ListEntry : public UGUIS_ButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget, meta=(DeprecatedFunction, DeprecationMessage="Use methods inside Game UI Subsystem"))
	void Bp_RegisterUIActionBinding(FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget, meta=(DeprecatedFunction, DeprecationMessage="Use methods inside Game UI Subsystem"))
	void Bp_RegisterEntryActionBinding(UGUIS_UIAction* EntryAction, bool bShouldDisplayInActionBar, const FGUIS_UIEntryActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget, meta=(DeprecatedFunction, DeprecationMessage="Use methods inside Game UI Subsystem"))
	void UnregisterBinding(UPARAM(ref)
		FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget, meta=(DeprecatedFunction))
	void UnregisterAllBindings();
};
