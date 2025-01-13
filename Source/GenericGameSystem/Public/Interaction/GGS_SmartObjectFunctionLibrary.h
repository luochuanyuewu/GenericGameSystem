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

	UFUNCTION(BlueprintCallable,Category="GGS",meta=(WorldContext="WorldContext", ExpandBoolAsExecs="ReturnValue"))
	static bool FindInteractionDefinitionFromSlot(UObject* WorldContext, FSmartObjectSlotHandle SmartObjectSlotHandle,FGGS_SmartObjectInteractionEntryData& OutDefinition);
};
