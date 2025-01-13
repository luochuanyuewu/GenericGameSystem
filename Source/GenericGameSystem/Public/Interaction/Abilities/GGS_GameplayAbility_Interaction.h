// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GGS_GameplayAbility_Interaction.generated.h"

class USmartObjectComponent;

/**
 * Base interaction ability. 
 */
UCLASS()
class GENERICGAMESYSTEM_API UGGS_GameplayAbility_Interaction : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION(BlueprintNativeEvent,Category="GCS|Interaction")
	void OnInteractActorChanged(AActor* OldActor, AActor* NewActor);


	UPROPERTY()
	USmartObjectComponent* CurrentSO;
};
