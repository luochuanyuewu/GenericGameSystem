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
	bTransientIsTriggering = true;
	bTransientIsActive = false;
	TransientAvatar = &InAvatar;
	TransientSmartObjectOwner = SmartObjectOwner;

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

	TSubclassOf<UGameplayAbility> AbilityClass{nullptr};
	int32 AbilityLevel = 0;
	if (!CheckValidAbilitySetting(Config, AbilityClass, AbilityLevel))
	{
		return false;
	}

	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
	{
		INTERACTION_RLOG(Error, TEXT("Try granting repeated interaction ability of class:%s, which is not supported!"), *AbilityClass->GetName())
		return false;
	}

	GrantedAbilityClass = AbilityClass;

	AbilityEndedDelegateHandle = Asc->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEndedCallback);

	//Ability trigger by event when activation polciy=ServerInitied won't work.
	AbilitySpecHandle = Asc->K2_GiveAbilityAndActivateOnce(AbilityClass, AbilityLevel);

	if (!AbilitySpecHandle.IsValid())
	{
		INTERACTION_RLOG(Error, TEXT("Can't active ability of class:%s! Check ability settings!"), *AbilityClass->GetName())
		return false;
	}

	// Special case: behavior already interrupted
	if (bBehaviorWasInterrupted && AbilitySpecHandle.IsValid() && !bAbilityEnded)
	{
		Asc->ClearAbility(AbilitySpecHandle);
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

	bTransientIsTriggering = false;
	bTransientIsActive = true;
	return bTransientIsActive;
}

void UGGS_GameplayBehavior_InteractionWithAbility::EndBehavior(AActor& Avatar, const bool bInterrupted)
{
	INTERACTION_RLOG(Verbose, TEXT("Interaction behavior ended %s"), bInterrupted?TEXT("due to interruption!"):TEXT("normally!"))

	// clear ability stuff.
	if (UAbilitySystemComponent* Asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(&Avatar))
	{
		if (AbilityEndedDelegateHandle.IsValid())
		{
			Asc->OnAbilityEnded.Remove(AbilityEndedDelegateHandle);
			AbilityEndedDelegateHandle.Reset();
		}

		// Special case: behavior interrupting active ability, so cancel ability.
		if (bInterrupted && bTransientIsActive && !bAbilityEnded && AbilitySpecHandle.IsValid())
		{
			if (const FGameplayAbilitySpec* Spec = Asc->FindAbilitySpecFromHandle(AbilitySpecHandle))
			{
				INTERACTION_RLOG(Verbose, TEXT("Cancel ability(%s) because behavior was interrupted."), *Spec->Ability.GetClass()->GetName())
				Asc->CancelAbilityHandle(AbilitySpecHandle);
			}
		}

		if (bInterrupted && !bTransientIsActive && AbilitySpecHandle.IsValid())
		{
			Asc->ClearAbility(AbilitySpecHandle);
		}
	}

	Super::EndBehavior(Avatar, bInterrupted);

	bBehaviorWasInterrupted = bInterrupted;
}

bool UGGS_GameplayBehavior_InteractionWithAbility::CheckValidAbilitySetting(const UGameplayBehaviorConfig* Config, TSubclassOf<UGameplayAbility>& OutAbilityClass, int32& OutAbilityLevel)
{
	// Ability class validation.
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
	OutAbilityClass = AbilityClass;
	OutAbilityLevel = InteractionConfig->AbilityLevel;
	return true;
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
		bAbilityEnded = true;
		bAbilityWasCancelled = EndedData.bWasCancelled;

		// Special case: behavior already active and abilities ended, ending behavior normally.
		if (!bTransientIsTriggering && bTransientIsActive)
		{
			INTERACTION_RLOG(Verbose, TEXT("Interaction ability(%s) %s."), *EndedData.AbilityThatEnded.GetClass()->GetName(),
			                 EndedData.bWasCancelled?TEXT("was canceled"):TEXT("ended normally"))
			EndBehavior(*GetAvatar(), false);
		}
	}
}
