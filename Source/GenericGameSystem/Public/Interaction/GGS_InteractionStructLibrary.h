// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectTypes.h"
#include "UObject/Object.h"
#include "GGS_InteractionStructLibrary.generated.h"


class UAbilitySystemComponent;
class UGCS_InteractionDefinition;

USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_SmartObjectInteractionEntryData : public FSmartObjectDefinitionData
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag TriggeringTag;

	friend bool operator==(const FGGS_SmartObjectInteractionEntryData& Lhs, const FGGS_SmartObjectInteractionEntryData& RHS)
	{
		return  Lhs.TriggeringInputAction == RHS.TriggeringInputAction && Lhs.TriggeringTag == RHS.TriggeringTag;
	}

	friend bool operator!=(const FGGS_SmartObjectInteractionEntryData& Lhs, const FGGS_SmartObjectInteractionEntryData& RHS)
	{
		return !(Lhs == RHS);
	}
};


USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_InteractionInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FGGS_SmartObjectInteractionEntryData Definition;

	/** The ability system on the target that can be used for the TargetInteractionHandle and sending the event, if needed. */
	// UPROPERTY(BlueprintReadOnly)
	// TObjectPtr<UAbilitySystemComponent> TargetAbilitySystem = nullptr;

	/** The ability spec to activate on the object for this option. */
	// UPROPERTY(BlueprintReadOnly)
	// FGameplayAbilitySpecHandle TargetInteractionAbilityHandle;

	/**
	 * The request result associated with this option.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FSmartObjectRequestResult RequestResult;

	/**
	 * The behavior definition associated with this option.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<const USmartObjectBehaviorDefinition> BehaviorDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	int32 SlotIndex{-1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	ESmartObjectSlotState SlotState{ESmartObjectSlotState::Free};

	FDelegateHandle DelegateHandle;

	friend bool operator==(const FGGS_InteractionInstance& Lhs, const FGGS_InteractionInstance& RHS)
	{
		return Lhs.Definition == RHS.Definition
			// && Lhs.TargetAbilitySystem == RHS.TargetAbilitySystem
			// && Lhs.TargetInteractionAbilityHandle == RHS.TargetInteractionAbilityHandle
			&& Lhs.RequestResult == RHS.RequestResult
			&& Lhs.BehaviorDefinition == RHS.BehaviorDefinition
			&& Lhs.SlotIndex == RHS.SlotIndex
			&& Lhs.SlotState == RHS.SlotState;
	}

	friend bool operator!=(const FGGS_InteractionInstance& Lhs, const FGGS_InteractionInstance& RHS)
	{
		return !(Lhs == RHS);
	}

	friend bool operator<(const FGGS_InteractionInstance& Lhs, const FGGS_InteractionInstance& RHS)
	{
		return Lhs.SlotIndex < RHS.SlotIndex;
	}
};
