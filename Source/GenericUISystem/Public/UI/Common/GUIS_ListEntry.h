// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
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
 * ListEntry will represent a list item(Item of ListView/TileView)
 */
UCLASS(Abstract, meta = (Category = "Generic UI", DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_ListEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void Bp_RegisterUIActionBinding(FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void Bp_RegisterEntryActionBinding(UGUIS_UIAction* EntryAction, bool bShouldDisplayInActionBar, const FGUIS_UIEntryActionExecutedDelegate& Callback, FGUIS_UIActionBindingHandle& BindingHandle);

	// UFUNCTION(BlueprintNativeEvent, Category = ExtendedActivatableWidget)
	// void HandleEntryAction(const UGUIS_EntryAction* EntryAction) ;

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterBinding(UPARAM(ref) FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterAllBindings();

	virtual void NativeDestruct() override;

private:
	TArray<FUIActionBindingHandle> BindingHandles;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = ExtendedActivatableWidget)
	// TArray<TObjectPtr<UGUIS_EntryAction>> EntryActionsWhenSelected;
	// UPROPERTY(EditAnywhere, Instanced, Category = ExtendedActivatableWidget)
	// TArray<TObjectPtr<UGUIS_EntryAction>> EntryActionsOnSelected;
	//
	// virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// virtual void NativeOnEntryReleased() override;
	// virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
};
