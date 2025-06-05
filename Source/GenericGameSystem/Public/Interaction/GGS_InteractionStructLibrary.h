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

/**
 * A simple struct warps around an instanced interaction definition object as interaction entrance.
 * 一个简单的结构体，报了一个实例化的交互定义对象，视作交互入口。
 */
USTRUCT(DisplayName="Interaction Entrance")
struct GENERICGAMESYSTEM_API FGGS_SmartObjectInteractionEntranceData : public FSmartObjectDefinitionData
{
	GENERATED_BODY()

	/**
	 * Customizable object contains any static information required for player interaction.
	 * @attention It's replicated cross network.
	 * 可自定义的对象，包含玩家交互所需的所有静态数据。
	 * @注意 它通过网络进行同步。
	 */
	UPROPERTY(EditAnywhere, Category="Interaction", meta=(DisplayName="Definition"))
	TSoftObjectPtr<UGGS_InteractionDefinition> DefinitionDA{nullptr};
};

/**
 * A piece of data represents an interaction option.
 * 用于表示一个交互选项的数据。
 */
USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_InteractionOption
{
	GENERATED_BODY()

	/**
	 * The interaction definition data associated with this option.
	 * 与此交互选项关联的交互定义数据。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<UGGS_InteractionDefinition> Definition{nullptr};

	/**
	 * The smart object associated with this option. Not replicated
	 * 与此交互选项关联的智能对象及其槽。未网络同步
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated, Category="Interaction")
	FSmartObjectRequestResult RequestResult;

	/**
	 * The smart object behavior definition associated with this option.
	 * 与此交互选项关联的智能对象行为定义。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NotReplicated, Category="Interaction")
	TObjectPtr<const USmartObjectBehaviorDefinition> BehaviorDefinition;

	/**
	 * The index of the associated smart object slot. Can be used as a sorter for ui listing.
	 * 与此交互选项关联的智能对象槽的Index。可用于在UI交互列表中排序。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	int32 SlotIndex{-1};

	/**
	 * The state of the associated smart object slot. Can be used as rules to check ui input.
	 * 与此交互选项关联的智能对象槽的状态。可用于控制UI交互规则。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	ESmartObjectSlotState SlotState{ESmartObjectSlotState::Free};

	friend bool operator==(const FGGS_InteractionOption& Lhs, const FGGS_InteractionOption& RHS)
	{
		return Lhs.Definition == RHS.Definition
			&& Lhs.RequestResult == RHS.RequestResult
			&& Lhs.SlotIndex == RHS.SlotIndex
			&& Lhs.SlotState == RHS.SlotState;
	}

	friend bool operator!=(const FGGS_InteractionOption& Lhs, const FGGS_InteractionOption& RHS)
	{
		return !(Lhs == RHS);
	}

	friend bool operator<(const FGGS_InteractionOption& Lhs, const FGGS_InteractionOption& RHS)
	{
		return Lhs.SlotIndex < RHS.SlotIndex;
	}

	FString ToString() const;
};
