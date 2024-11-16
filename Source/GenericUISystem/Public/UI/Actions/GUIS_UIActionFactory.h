// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUIS_UIAction.h"
#include "UObject/Object.h"
#include "GUIS_UIActionFactory.generated.h"

/**
 *  提供一种通用的方式为UI对象选择合适的可用操作。
 */
UCLASS(BlueprintType, Const)
class GENERICUISYSTEM_API UGUIS_UIActionFactory : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GUIS|UIAction")
	TArray<FGUIS_UIActionDefinition> FindUIActionsForData(const UObject *Data) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS|UIAction")
	TArray<FGUIS_UIActionDefinition> UIActionDefinitions;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;
#endif
};
