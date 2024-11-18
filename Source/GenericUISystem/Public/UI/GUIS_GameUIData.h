// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Engine/DataAsset.h"
#include "GUIS_GameUIData.generated.h"

class UGUIS_GameModalWidget;

/**
 * 主要数据资产。
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_GameUIData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GUIS")
	TSoftClassPtr<UGUIS_GameModalWidget> FindWidgetClassForModal(FGameplayTag ModelTag) const;

protected:
	/**
	 * Mapping of ModalTag to Modal widget class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS|Modal", meta = (ForceInlineRow))
	TMap<FGameplayTag, TSoftClassPtr<UGUIS_GameModalWidget>> TaggedModalWidgetClass;
};
