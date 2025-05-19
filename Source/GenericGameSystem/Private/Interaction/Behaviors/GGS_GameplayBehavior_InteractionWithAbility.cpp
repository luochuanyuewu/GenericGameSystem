// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/Behaviors/GGS_GameplayBehavior_InteractionWithAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GGS_LogChannels.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/Behaviors/GGS_GameplayBehaviorConfig_InteractionWithAbility.h"
#include "Interaction/GGS_InteractionSystemComponent.h"

bool UGGS_GameplayBehavior_InteractionWithAbility::Trigger(AActor& InAvatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner)
{
	Super::Trigger(InAvatar, Config, SmartObjectOwner);
	const UGGS_GameplayBehaviorConfig_InteractionWithAbility* InteractionConfig = Cast<const UGGS_GameplayBehaviorConfig_InteractionWithAbility>(Config);
	if (!InteractionConfig)
	{
		INTERACTION_RLOG(Error, TEXT("Invalid GameplayBehaviorConfig! expect Config be type of %s."), *UGGS_GameplayBehaviorConfig_InteractionWithAbility::StaticClass()->GetName())
		return false;
	}

	const TSubclassOf<UGameplayAbility> AbilityClass = InteractionConfig->AbilityToGrant.LoadSynchronous();
	if (!AbilityClass)
	{
		INTERACTION_RLOG(Error, TEXT("Invalid AbilityToGrant Class!"))
		return false;
	}

	UGGS_InteractionSystemComponent* InteractionSystem = InAvatar.FindComponentByClass<UGGS_InteractionSystemComponent>();

	if (!InteractionSystem)
	{
		INTERACTION_RLOG(Error, TEXT("Missing InteractionSystem Component!"))
		return false;
	}

	UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&InAvatar);
	if (!Asc)
	{
		INTERACTION_RLOG(Error, TEXT("Missing Ability System Component!"))
		return false;
	}

	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
	{
		INTERACTION_RLOG(Error, TEXT("Try granting repeated interaction ability of class:%s, which is not supported!"), *AbilityClass->GetName())
		return false;
	}

	// build and validate the ability spec
	FGameplayAbilitySpec AbilitySpec = Asc->BuildAbilitySpecFromClass(AbilityClass, InteractionConfig->AbilityLevel);

	// validate the class
	if (!IsValid(AbilitySpec.Ability))
	{
		INTERACTION_RLOG(Error, TEXT("Failed to create instance of ability:%s"), *AbilityClass->GetName())
		return false;
	}

	GrantedAbilityClass = AbilitySpec.Ability->GetClass();

	// FGameplayEventData EventData;
	// EventData.EventTag = InteractionConfig->AbilityTriggeringTag;
	// EventData.Instigator = InAvatar;
	// EventData.Target = SmartObjectOwner;

	AbilityEndedDelegateHandle = Asc->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEndedCallback);

	//Ability trigger by event when activation polciy=ServerInitied won't work.
	AbilitySpecHandle = Asc->GiveAbilityAndActivateOnce(AbilitySpec, nullptr);

	if (!AbilitySpecHandle.IsValid())
	{
		INTERACTION_RLOG(Error, TEXT("Can't active ability of class:%s! Check ability settings!"), *AbilityClass->GetName())
		return false;
	}

	if (AbilitySpecHandle.IsValid() && bAbilityEnded)
	{
		INTERACTION_RLOG(Verbose, TEXT("Instantly executed interaction ability:%s,handle%s"), *AbilityClass->GetName(), *AbilitySpecHandle.ToString())
		EndBehavior(InAvatar, false);
		return false;
	}

	INTERACTION_RLOG(Verbose, TEXT("Granted and activate interaction ability:%s,handle%s"), *AbilityClass->GetName(), *AbilitySpecHandle.ToString())

	//TODO what happens when avatar or target get destoryied?
	// SOOwner销毁的时候, 需要Abort当前行为, 目的是清除赋予的Ability
	// SmartObjectOwner->OnDestroyed.AddDynamic(this, &ThisClass::OnSmartObjectOwnerDestroyed);
	INTERACTION_RLOG(Verbose, TEXT("Interaction begins with ability:%s"), *AbilityClass->GetName())
	return true;
}

void UGGS_GameplayBehavior_InteractionWithAbility::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	Super::EndBehavior(Avatar, bInterrupted);

	INTERACTION_RLOG(Verbose, TEXT("Interaction finished %s"), bInterrupted?TEXT("due to interruption!"):TEXT("normally!"))

	UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&Avatar);

	// unregister ability ended callback.
	if (Asc)
	{
		if (AbilityEndedDelegateHandle.IsValid())
		{
			Asc->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
			AbilityEndedDelegateHandle.Reset();
		}
		// cancel ability if ability is not ended.
		if (!bAbilityEnded && AbilitySpecHandle.IsValid())
		{
			if (const FGameplayAbilitySpec* Spec = Asc->FindAbilitySpecFromHandle(AbilitySpecHandle))
			{
				INTERACTION_RLOG(Verbose, TEXT("Cancel ability(%s) because behavior ended early."), *Spec->Ability.GetClass()->GetName())
				Asc->CancelAbilityHandle(AbilitySpecHandle);
			}
		}
	}
}

void UGGS_GameplayBehavior_InteractionWithAbility::OnAbilityEndedCallback(const FAbilityEndedData& EndedData)
{
	if (bAbilityEnded)
	{
		return;
	}

	// check for ability granted by this behavior.
	if (EndedData.AbilitySpecHandle == AbilitySpecHandle || EndedData.AbilityThatEnded->GetClass() == GrantedAbilityClass)
	{
		INTERACTION_RLOG(Verbose, TEXT("Interaction ability(%s) %s, end behavior."), *EndedData.AbilityThatEnded.GetClass()->GetName(),
		                 EndedData.bWasCancelled?TEXT("was canceled"):TEXT("ended normally"))
		bAbilityEnded = true;
		EndBehavior(*GetAvatar(), EndedData.bWasCancelled);
	}
}
