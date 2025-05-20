// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GGS_InteractionDefinition.generated.h"

/**
 * Base class for interaction settings.
 * Subclass from it to add new fields. Create an instance of this data asset to be used within SmartObjectDefinition's interaction entrence.
 * 交互设置的基类。
 * 集成以添加新字段。创建该数据资产实例以在智能对象定义的交互入口中引用。
 */
UCLASS(BlueprintType,Blueprintable)
class GENERICGAMESYSTEM_API UGGS_InteractionDefinition : public UDataAsset
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
