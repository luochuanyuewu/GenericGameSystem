// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Engine/DataTable.h"
#include "GUIS_GameModalTypes.generated.h"

class UGUIS_ButtonBase;
class FSubsystemCollectionBase;
class UGUIS_ModalDefinition;
class UObject;


DECLARE_DELEGATE_OneParam(FGUIS_ModalActionResultSignature, FGameplayTag /* Result */);

/**
 * A modal action configuration.
 * 消息框操作配置.
 */
USTRUCT(BlueprintType)
struct FGUIS_GameModalAction
{
	GENERATED_BODY()

public:
	/**
	 * Display Text to use instead of the action name associated with the result.
	 * 此操作的名称
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	FText DisplayText;

	/**
	 * The button widget used to represent this modal action.
	 * 用什么按钮展示此操作。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", NoClear)
	TSoftClassPtr<UGUIS_ButtonBase> ButtonType;

	/**
	 * The common ui input associated with this modal action.
	 * 此操作关联的输入。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputAction;
};
