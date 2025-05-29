// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectRuntime.h"
#include "Abilities/GameplayAbility.h"
#include "GGS_GameplayAbility_Interaction.generated.h"

class UGGS_InteractionSystemComponent;
class USmartObjectComponent;

/**
 * Core interaction ability. 
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICGAMESYSTEM_API UGGS_GameplayAbility_Interaction : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGGS_GameplayAbility_Interaction();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	UFUNCTION(BlueprintCallable, Category="GCS|Interaction", meta=(ExpandBoolAsExecs=ReturnValue))
	bool TryClaimInteraction(int32 Index, FSmartObjectClaimHandle& ClaimedHandle);

	UFUNCTION(BlueprintNativeEvent, Category="GCS|Interaction")
	void OnInteractActorChanged(AActor* OldActor, AActor* NewActor);

	UPROPERTY(BlueprintReadOnly, Category="GCS|Interaction")
	TObjectPtr<UGGS_InteractionSystemComponent> InteractionSystem{nullptr};

#if WITH_EDITORONLY_DATA
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
