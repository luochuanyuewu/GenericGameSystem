// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGS_InteractionStructLibrary.h"
#include "SmartObjectSubsystem.h"
#include "SmartObjectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GGS_SmartObjectFunctionLibrary.generated.h"

class UGameplayInteractionSmartObjectBehaviorDefinition;
class UGameplayBehaviorSmartObjectBehaviorDefinition;
class UGameplayBehaviorConfig;
class USmartObjectBehaviorDefinition;

/**
 * 
 */
UCLASS()
class GENERICGAMESYSTEM_API UGGS_SmartObjectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get GameplayBehaviorConfig from a smart object behavior definition.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GGS|SmartObject")
	static UGameplayBehaviorConfig* GetGameplayBehaviorConfig(const USmartObjectBehaviorDefinition* BehaviorDefinition);

	UFUNCTION(BlueprintCallable, Category = "GGS|SmartObject", meta=(DeterminesOutputType="DesiredClass", DynamicOutputParam="OutConfig", ExpandBoolAsExecs="ReturnValue"))
	static bool FindGameplayBehaviorConfig(const USmartObjectBehaviorDefinition* BehaviorDefinition, TSubclassOf<UGameplayBehaviorConfig> DesiredClass, UGameplayBehaviorConfig*& OutConfig);

	/**
	 * Search a given Actor for slot candidates respecting the request criteria and selection conditions, also with interaction entrance.
	 * 根据指定的过滤器，在给定Actor上搜索带有交互入口的槽。
	 * @param Filter Parameters defining the search area and criteria
	 * @param SearchActor The actor to search
	 * @param OutResults List of smart object slot candidates found in range
	 * @param UserActor  Used to create additional data that could be provided to bind values in the conditions evaluation context
	 * @return True if at least one candidate was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = False, Category = "SmartObject", Meta = (ReturnDisplayName = "bSuccess"))
	static bool FindSmartObjectsWithInteractionEntranceInActor(const FSmartObjectRequestFilter& Filter, AActor* SearchActor, TArray<FSmartObjectRequestResult>& OutResults,
	                                                           const AActor* UserActor = nullptr);

	/**
	 * Try finding an interaction definition in SmartObjectSlot.
	 * @param WorldContext 
	 * @param SmartObjectSlotHandle The slot.
	 * @param OutDefinition Interaction definition for player.
	 * @return true means this slot has associated interaction definition(player interactable). false means this slot has no valid interaction entry for players.
	 */
	UFUNCTION(BlueprintCallable, Category="GGS", meta=(WorldContext="WorldContext", ExpandBoolAsExecs="ReturnValue"))
	static bool FindInteractionDefinitionFromSmartObjectSlot(UObject* WorldContext, FSmartObjectSlotHandle SmartObjectSlotHandle, UGGS_InteractionDefinition*& OutDefinition);
};
