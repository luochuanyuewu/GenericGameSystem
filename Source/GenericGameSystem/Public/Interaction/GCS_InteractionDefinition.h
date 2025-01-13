// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GCS_InteractionDefinition.generated.h"

/**
 * Base class for interaction settings.
 */
UCLASS(Blueprintable, BlueprintType)
class GENERICGAMESYSTEM_API UGCS_InteractionDefinition : public UDataAsset
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
};
