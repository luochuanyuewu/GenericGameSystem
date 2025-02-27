// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GUIS_UIAction.generated.h"

class UGUIS_ModalDefinition;
class UGUIS_UIAction;

USTRUCT(BlueprintType)
struct FGUIS_UIActionDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	FName ActionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	bool bShouldDisplayInActionBar{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", Instanced)
	TObjectPtr<const UGUIS_UIAction> EntryAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	bool bRequiresConfirm{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS",meta=(EditCondition="bRequiresConfirm"))
	TSoftClassPtr<UGUIS_ModalDefinition> Confirmation{nullptr};

	bool operator ==(const FName& OtherActionId) const;
};

/**
 *
 */
UCLASS(Blueprintable, EditInlineNew, CollapseCategories, DefaultToInstanced, Abstract, Const)
class GENERICUISYSTEM_API UGUIS_UIAction : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	bool IsCompatible(const UObject* Data) const;

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	bool CanInvoke(const UObject* Data, APlayerController* PlayerController) const;

	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	void InvokeAction(const UObject* Data, APlayerController* PlayerController) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|UIAction")
	FText GetActionName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|UIAction")
	FName GetActionID() const;

	const FDataTableRowHandle& GetInputActionData() const { return InputActionData; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "UIAction", meta = (DisplayName = "Is Combatible"))
	bool IsCompatibleInternal(const UObject* Data) const;

	UFUNCTION(BlueprintNativeEvent, Category = "UIAction", meta = (DisplayName = "Can Invoke"))
	bool CanInvokeInternal(const UObject* Data, APlayerController* PlayerController) const;

	UFUNCTION(BlueprintNativeEvent, Category = "UIAction", meta = (DisplayName = "Invoke Action"))
	void InvokeActionInternal(const UObject* Data, APlayerController* PlayerController) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UIAction")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UIAction")
	FName ActionID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UIAction", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputActionData;
};
