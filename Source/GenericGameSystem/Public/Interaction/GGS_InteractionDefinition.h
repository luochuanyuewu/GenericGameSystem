// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GGS_InteractionDefinition.generated.h"

/**
 * Base class for interaction settings.
 * 交互设置的基类。
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, Const)
class GENERICGAMESYSTEM_API UGGS_InteractionDefinition : public UObject
{
	GENERATED_BODY()

public:
	/** Simple text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText SubText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle TriggeringInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FGameplayTag TriggeringTag;
};
