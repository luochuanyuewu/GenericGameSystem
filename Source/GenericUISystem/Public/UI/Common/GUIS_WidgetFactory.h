// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GUIS_WidgetFactory.generated.h"

/**
 *  提供一种通用的方式为任意对象选择合适的WidgetClass。
 *  Subclass from it.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, HideDropdown, Const)
class GENERICUISYSTEM_API UGUIS_WidgetFactory : public UDataAsset
{
	GENERATED_BODY()

public:
	UGUIS_WidgetFactory()
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="GUIS")
	TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject *Data) const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category="GUIS")
	bool OnDataValidation(FText &ValidationMessage) const;
	virtual bool OnDataValidation_Implementation(FText &ValidationMessage) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;
#endif
};
