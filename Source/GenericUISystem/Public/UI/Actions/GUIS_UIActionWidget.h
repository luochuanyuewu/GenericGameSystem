// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "GUIS_UIAction.h"
#include "GUIS_UIActionWidget.generated.h"

class UGUIS_AsyncAction_ShowModel;
class UGUIS_UIActionFactory;


/**
 * A widget which can associate data with register ui action dynamically based on passed-in data.
 * @attention There's no visual for this widget.
 * 此Widget可以关联一个数据，并为其自动注册可用的输入事件。
 */
UCLASS(ClassGroup = GUIS, meta=(Category="Generic UI System"), AutoExpandCategories=("GUIS"))
class GENERICUISYSTEM_API UGUIS_UIActionWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	void SetAssociatedData(UObject* Data);

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	void RegisterActions();

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	void UnregisterActions();

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	void CancelAction();


	//~ Begin UWidget
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End UWidget interface

protected:
	UFUNCTION()
	virtual void HandleUIAction(const FGUIS_UIActionDefinition& Definition);

	UFUNCTION()
	void HandleUIActionImmediately(const FGUIS_UIActionDefinition& Definition);

	UFUNCTION()
	virtual void HandleModalAction(FGameplayTag ActionTag); 

	UPROPERTY()
	TWeakObjectPtr<UObject> AssociatedData;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TArray<FGUIS_UIActionDefinition> ActionDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UIAction")
	TObjectPtr<UGUIS_UIActionFactory> ActionFactory;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGUIS_HandleUIActionSignature, FGUIS_UIActionDefinition, Definition);

	UPROPERTY(BlueprintAssignable, Category="GUIS|UIAction")
	FGUIS_HandleUIActionSignature OnHandleUIAction;

private:
	TArray<FUIActionBindingHandle> ActionBindings;

	UPROPERTY()
	FGUIS_UIActionDefinition CurrentDefinition;
		
	UPROPERTY()
	TObjectPtr<UGUIS_AsyncAction_ShowModel> ModalTask{nullptr};
};

