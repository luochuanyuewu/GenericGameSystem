// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_GameplayBehavior_InteractionWithAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GGS_LogChannels.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/GGS_GameplayBehaviorConfig_InteractionWithAbility.h"
#include "Interaction/GGS_InteractionSystemComponent.h"

bool UGGS_GameplayBehavior_InteractionWithAbility::Trigger(AActor& InAvatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner)
{
	Super::Trigger(InAvatar, Config, SmartObjectOwner);
	const UGGS_GameplayBehaviorConfig_InteractionWithAbility* InteractionConfig = Cast<const UGGS_GameplayBehaviorConfig_InteractionWithAbility>(Config);
	if (!InteractionConfig)
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to GameplayBehaviorConfig being null"), *InAvatar.GetName(), *GetClass()->GetName());
		return false;
	}

	const TSubclassOf<UGameplayAbility> AbilityClass = InteractionConfig->AbilityToGrant.LoadSynchronous();
	if (!AbilityClass)
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to InteractionAbilityToGrant being null"), *InAvatar.GetName(), *GetClass()->GetName());
		return false;
	}

	if (!InteractionConfig->AbilityTriggeringTag.IsValid())
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to AbilityTriggeringTag being empty"), *InAvatar.GetName(), *GetClass()->GetName());
		return false;
	}

	UGGS_InteractionSystemComponent* InteractionSystem = InAvatar.FindComponentByClass<UGGS_InteractionSystemComponent>();
	
	if (!InteractionSystem)
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to InteractionSystem being null"), *InAvatar.GetName(), *GetClass()->GetName());
		return false;
	}

	UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&InAvatar);
	if (!Asc)
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to Asc being null"),*GetClass()->GetName(), *InAvatar.GetName());
		return false;
	}

	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to repeat adding ability class:%s"), *InAvatar.GetName(), *GetClass()->GetName(),
		       *AbilityClass->GetName());
		return false;
	}

	// build and validate the ability spec
	FGameplayAbilitySpec AbilitySpec = Asc->BuildAbilitySpecFromClass(AbilityClass, InteractionConfig->AbilityLevel);

	// validate the class
	if (!IsValid(AbilitySpec.Ability))
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s due to invalid ability class:%s"), *InAvatar.GetName(), *GetClass()->GetName(), *AbilityClass->GetName());
		return false;
	}

	FGameplayEventData EventData;
	EventData.EventTag = InteractionConfig->AbilityTriggeringTag;
	EventData.Instigator = InAvatar;
	EventData.Target = SmartObjectOwner;

	AbilityEndedDelegateHandle = Asc->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEndedCallback);

	AbilitySpecHandle = Asc->GiveAbilityAndActivateOnce(AbilitySpec, &EventData);
	if (!AbilitySpecHandle.IsValid())
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to trigger GameplayBehavior:%s, can't active ability:%s"), *InAvatar.GetName(), *GetClass()->GetName(), *AbilityClass->GetName());
		EndBehavior(InAvatar, true);
		return false;
	}
	//TODO what happens when avatar or target get destoryied?
	// SOOwner销毁的时候, 需要Abort当前行为, 目的是清除赋予的Ability
	// SmartObjectOwner->OnDestroyed.AddDynamic(this, &ThisClass::OnSmartObjectOwnerDestroyed);

	UE_LOG(LogGGS_Interaction, Display, TEXT("%s successfully triggered GameplayBehavior:%s, interaction actor:%s"), *InAvatar.GetName(), *GetClass()->GetName(),
	       *SmartObjectOwner->GetName());
	return true;
}

void UGGS_GameplayBehavior_InteractionWithAbility::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	Super::EndBehavior(Avatar, bInterrupted);

	UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&Avatar);

	// unregister ability ended callback.
	if (Asc)
	{
		if (AbilityEndedDelegateHandle.IsValid())
		{
			Asc->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
			AbilityEndedDelegateHandle.Reset();
		}
	}
}

void UGGS_GameplayBehavior_InteractionWithAbility::OnAbilityEndedCallback(const FAbilityEndedData& EndedData)
{
	if (bAbilityEnded)
		return;

	// check for ability granted by this behavior.
	if (EndedData.AbilitySpecHandle == AbilitySpecHandle)
	{
		UE_LOG(LogGGS_Interaction, Display, TEXT("Interaction Ability(ClassName:%s) ended,so ending behavior normally."), *EndedData.AbilityThatEnded->GetName());
		bAbilityEnded = true;
		EndBehavior(*GetAvatar(), EndedData.bWasCancelled);
	}
}
