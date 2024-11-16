// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GUIS_UIAction.h"
#include "GUIS_UIActionWidget.generated.h"

class UGUIS_UIActionFactory;


/**
 */
UCLASS(ClassGroup = GUIS, meta=(Category="Generic UI System"))
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


	//~ Begin UWidget
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End UWidget interface

protected:
	UFUNCTION()
	void HandleUIAction(const FGUIS_UIActionDefinition& Definition);

	UPROPERTY()
	TWeakObjectPtr<UObject> AssociatedData;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TArray<FGUIS_UIActionDefinition> ActionDefinitions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UIAction")
	TObjectPtr<UGUIS_UIActionFactory> ActionFactory;

private:
	TArray<FUIActionBindingHandle> ActionBindings;
};
