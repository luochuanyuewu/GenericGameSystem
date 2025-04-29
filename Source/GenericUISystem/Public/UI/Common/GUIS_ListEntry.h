// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Foundation/GUIS_ButtonBase.h"
#include "GUIS_ListEntry.generated.h"

class UGUIS_UIAction;
DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIActionExecutedDelegate, FName, ActionName);

DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIEntryActionExecutedDelegate, UGUIS_UIAction *, EntryAction);

USTRUCT(BlueprintType)
struct FGUIS_UIActionBindingHandle
{
	GENERATED_BODY()

	FName Id;

	FUIActionBindingHandle Handle;
};

/**
 * ListEntry(as CommonButton) will represent a list item(Item of ListView/TileView)
 */
UCLASS(Abstract, meta = (Category = "Generic UI", DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_ListEntry : public UGUIS_ButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void Bp_RegisterUIActionBinding(FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void Bp_RegisterEntryActionBinding(UGUIS_UIAction* EntryAction, bool bShouldDisplayInActionBar, const FGUIS_UIEntryActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterBinding(UPARAM(ref) FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterAllBindings();

	virtual void NativeDestruct() override;

private:
	TArray<FUIActionBindingHandle> BindingHandles;
};
