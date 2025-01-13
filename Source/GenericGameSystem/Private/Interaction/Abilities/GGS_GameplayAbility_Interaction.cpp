// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/Abilities/GGS_GameplayAbility_Interaction.h"

#include "Interaction/GGS_InteractionSystemComponent.h"

void UGGS_GameplayAbility_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UGGS_InteractionSystemComponent* UserComponent = UGGS_InteractionSystemComponent::GetInteractionSystemComponent(ActorInfo->AvatarActor.Get()) )
	{
		UserComponent->OnInteractionActorChangedEvent.AddDynamic(this,&ThisClass::OnInteractActorChanged);
	}
}

void UGGS_GameplayAbility_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UGGS_InteractionSystemComponent* UserComponent = UGGS_InteractionSystemComponent::GetInteractionSystemComponent(ActorInfo->AvatarActor.Get()) )
	{
		UserComponent->OnInteractionActorChangedEvent.RemoveDynamic(this,&ThisClass::OnInteractActorChanged);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGGS_GameplayAbility_Interaction::OnInteractActorChanged_Implementation(AActor* OldActor, AActor* NewActor)
{
}
