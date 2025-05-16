// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectTypes.h"
#include "Engine/DataTable.h"
#include "UObject/Object.h"
#include "GGS_InteractionStructLibrary.generated.h"


class UGGS_InteractionSystemComponent;
class UAbilitySystemComponent;
class UGGS_InteractionDefinition;

USTRUCT(BlueprintType,DisplayName="Interaction Entry")
struct GENERICGAMESYSTEM_API FGGS_SmartObjectInteractionEntryData : public FSmartObjectDefinitionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction",Instanced)
	TObjectPtr<UGGS_InteractionDefinition> Definition;
};


USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_InteractionInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction",meta=(DisplayName="Definition"))
	TObjectPtr<UGGS_InteractionDefinition> Definition{nullptr};

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
