// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"

#include "GUIS_GameModalTypes.generated.h"

class UGUIS_ButtonBase;
class FSubsystemCollectionBase;
class UGUIS_ModalDefinition;
class UObject;


DECLARE_DELEGATE_OneParam(FGUIS_ModalActionResultSignature, FGameplayTag /* Result */);

/**
 * 确认对话框动作.
 */
USTRUCT(BlueprintType)
struct FGUIS_GameModalAction
{
	GENERATED_BODY()

public:
	/** Display Text to use instead of the action name associated with the result. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	FText DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", NoClear)
	TSoftClassPtr<UGUIS_ButtonBase> ButtonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputAction;
};
