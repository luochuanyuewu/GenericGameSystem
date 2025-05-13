// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_InteractionSystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/World.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorSubsystem.h"
#include "GGS_LogChannels.h"
#include "SmartObjectBlueprintFunctionLibrary.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"
#include "Interaction/GGS_GameplayAbilitiesSmartObjectBehaviorDefinition.h"
#include "Interaction/GGS_GameplayBehaviorConfig_InteractionWithAbility.h"
#include "Interaction/GGS_SmartObjectFunctionLibrary.h"
#include "Interaction/GGS_InteractionStructLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values for this component's properties
UGGS_InteractionSystemComponent::UGGS_InteractionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

void UGGS_InteractionSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InteractActor, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bInteracting, Params);
	DOREPLIFETIME_CONDITION(ThisClass, InteractionInstances, COND_OwnerOnly);
}

UGGS_InteractionSystemComponent* UGGS_InteractionSystemComponent::GetInteractionSystemComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UGGS_InteractionSystemComponent>() : nullptr;
}


// Called every frame
void UGGS_InteractionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGGS_InteractionSystemComponent::CycleInteractActors_Implementation(bool bNext)
{
	if (bInteracting || PotentialActors.Num() <= 1)
	{
		return;
	}

	int32 Index = InteractActor != nullptr ? PotentialActors.IndexOfByKey(InteractActor) : 0;
	if (!PotentialActors.IsValidIndex(Index)) //一个都没
	{
		return;
	}
	if (bNext)
	{
		Index = FMath::Clamp(Index + 1, 0, PotentialActors.Num());
	}
	else
	{
		Index = FMath::Clamp(Index - 1, 0, PotentialActors.Num());
	}
	if (PotentialActors.IsValidIndex(Index) && PotentialActors[Index] != nullptr && PotentialActors[Index] !=
		InteractActor)
	{
		SetInteractActor(PotentialActors[Index]);
	}
}

void UGGS_InteractionSystemComponent::SetPotentialActors(TArray<AActor*> NewActors)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	PotentialActors = NewActors;

	if (!bInteracting)
	{
		// update potential actor.
		if (!IsValid(InteractActor) || !PotentialActors.Contains(InteractActor))
		{
			if (NewActors.IsValidIndex(0) && IsValid(NewActors[0]))
			{
				SetInteractActor(NewActors[0]);
			}
			else
			{
				SetInteractActor(nullptr);
			}
		}
	}
}

void UGGS_InteractionSystemComponent::SetInteractActor(AActor* InActor)
{
	if (InActor != InteractActor)
	{
		AActor* OldActor = InteractActor;
		InteractActor = InActor;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractActor, this)
		OnInteractActorChanged(OldActor);
	}
}

FSmartObjectRequestFilter UGGS_InteractionSystemComponent::GetSmartObjectRequestFilter_Implementation()
{
	return DefaultRequestFilter;
}

bool UGGS_InteractionSystemComponent::FindSmartObjectsInActor(AActor* InActor, TArray<FSmartObjectRequestResult>& OutResults)
{
	USmartObjectSubsystem* Subsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();

	if (!Subsystem)
	{
		return false;
	}

	TArray<FSmartObjectRequestResult> Results;
	USmartObjectBlueprintFunctionLibrary::FindSmartObjectsInActor(GetSmartObjectRequestFilter(), InActor, Results, GetOwner());

	if (Results.IsEmpty())
	{
		return false;
	}

	OutResults.Empty();

	for (FSmartObjectRequestResult& Result : Results)
	{
		FGGS_SmartObjectInteractionEntryData InteractionDefinition;
		if (UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSlot(this, Result.SlotHandle, InteractionDefinition))
		{
			OutResults.Add(Result);
		}
	}

	return !OutResults.IsEmpty();
}

void UGGS_InteractionSystemComponent::SetInteractionState(bool bNewState)
{
	bool prevState = bInteracting;
	COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, bInteracting, bNewState, this);
	OnInteractionStateChanged(prevState);
}

bool UGGS_InteractionSystemComponent::GetInteractionState() const
{
	return bInteracting;
}

// void UGGS_InteractionSystemComponent::TriggerInteraction(int32 Index)
// {
// 	TriggerInteraction(Index, true);
// }

void UGGS_InteractionSystemComponent::StartInteraction_Implementation(int32 Index)
{
	if (bInteracting)
	{
		return;
	}

	bool bSuccess = false;
	ON_SCOPE_EXIT
	{
		if (!bSuccess)
		{
			StopInteraction();
		}
	};


	bSuccess = InternalStartInteraction(Index);
}

void UGGS_InteractionSystemComponent::StopInteraction_Implementation()
{
	if (!bBehaviorFinished && GameplayBehavior != nullptr)
	{
		GameplayBehavior->GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		GameplayBehavior->AbortBehavior(*GetOwner());
	}
	if (ClaimedHandle.IsValid())
	{
		USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
		check(SmartObjectSubsystem);
		SmartObjectSubsystem->MarkSlotAsFree(ClaimedHandle);
		SmartObjectSubsystem->UnregisterSlotInvalidationCallback(ClaimedHandle);
		ClaimedHandle.Invalidate();
	}
	if (PotentialActors.IsEmpty())
	{
		SetInteractActor(nullptr);
	}
	SetInteractionState(false);
	GetOwner()->ForceNetUpdate();
}

bool UGGS_InteractionSystemComponent::InternalStartInteraction(int32 Index)
{
	USmartObjectSubsystem* Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());

	check(Subsystem!=nullptr)

	if (!InteractionInstances.IsValidIndex(Index))
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s failed to start interaction, Option at index(%d) not exist! %S"), *GetOwner()->GetName(), Index, __FUNCTION__);
		return false;
	}

	if (InteractionInstances[Index].SlotState != ESmartObjectSlotState::Free)
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s failed to start interaction for %s(index:%d), Target slot state != Free! %S"), *GetOwner()->GetName(),
		       *InteractionInstances[Index].Definition.Text.ToString(), Index, __FUNCTION__);
		return false;
	}

	FGGS_InteractionInstance& CurrentOption = InteractionInstances[Index];

	FSmartObjectClaimHandle NewlyClaimedHandle = USmartObjectBlueprintFunctionLibrary::MarkSmartObjectSlotAsClaimed(this, CurrentOption.RequestResult.SlotHandle, GetOwner());

	// A valid claimed handle can point to an object that is no longer part of the simulation
	if (!Subsystem->IsClaimedSmartObjectValid(NewlyClaimedHandle))
	{
		UE_LOG(LogGGS_Interaction, Error, TEXT("%s failed to start interaction for %s(index:%d), refers to an object that is no longer available. %S"), *GetOwner()->GetName(),
		       *InteractionInstances[Index].Definition.Text.ToString(), Index, __FUNCTION__);
		return false;
	}

	ClaimedHandle = NewlyClaimedHandle;
	// Register a callback to be notified if the claimed slot became unavailable
	Subsystem->RegisterSlotInvalidationCallback(ClaimedHandle, FOnSlotInvalidated::CreateUObject(this, &ThisClass::OnClaimedSlotInvalidated));

	const UGameplayBehaviorSmartObjectBehaviorDefinition* SmartObjectGameplayBehaviorDefinition = Subsystem->MarkSlotAsOccupied<
		UGameplayBehaviorSmartObjectBehaviorDefinition>(ClaimedHandle);
	CurrentOption.SlotState = Subsystem->GetSlotState(CurrentOption.RequestResult.SlotHandle);

	const UGameplayBehaviorConfig* GameplayBehaviorConfig = SmartObjectGameplayBehaviorDefinition != nullptr ? SmartObjectGameplayBehaviorDefinition->GameplayBehaviorConfig : nullptr;
	GameplayBehavior = GameplayBehaviorConfig != nullptr ? GameplayBehaviorConfig->GetBehavior(*GetWorld()) : nullptr;
	if (GameplayBehavior == nullptr)
	{
		return false;
	}

	const USmartObjectComponent* SmartObjectComponent = Subsystem->GetSmartObjectComponent(ClaimedHandle);
	AActor& Me = *GetOwner();
	AActor* Target = SmartObjectComponent ? SmartObjectComponent->GetOwner() : nullptr;
	const bool bBehaviorActive = UGameplayBehaviorSubsystem::TriggerBehavior(*GameplayBehavior, Me, GameplayBehaviorConfig, Target);
	// Behavior can be successfully triggered AND ended synchronously. We are only interested to register callback when still running
	if (bBehaviorActive)
	{
		OnBehaviorFinishedNotifyHandle = GameplayBehavior->GetOnBehaviorFinishedDelegate().AddUObject(this, &ThisClass::OnSmartObjectBehaviorFinished);
		SetInteractionState(true);
	}

	return bBehaviorActive;
}

// void UGGS_InteractionSystemComponent::TriggerInteraction(int32 Index, bool bSendRpc)
// {
// 	if (!InteractionInstances.IsValidIndex(Index))
// 	{
// 		return;
// 	}
//
// 	const FGGS_InteractionInstance& Instance = InteractionInstances[Index];
//
// 	if (Instance.BehaviorDefinition && Instance.BehaviorDefinition->GetClass()->IsChildOf(UGGS_GameplayAbilitiesSmartObjectBehaviorDefinition::StaticClass()))
// 	{
// 		// Allow the target to customize the event data we're about to pass in, in case the ability needs custom data
// 		// that only the actor knows.
// 		FGameplayEventData Payload;
// 		Payload.EventTag = Instance.Definition.TriggeringTag;
// 		Payload.Instigator = GetOwner();
// 		Payload.Target = InteractActor;
//
// 		// Trigger the ability using event tag.
// 		const bool bSuccess = Instance.TargetAbilitySystem->TriggerAbilityFromGameplayEvent(
// 			Instance.TargetInteractionAbilityHandle,
// 			Instance.TargetAbilitySystem->AbilityActorInfo.Get(),
// 			Instance.Definition.TriggeringTag,
// 			&Payload,
// 			*Instance.TargetAbilitySystem
// 		);
// 	}
//
// 	if (bSendRpc)
// 	{
// 		ServerTriggerInteraction(Index);
// 	}
// }

// void UGGS_InteractionSystemComponent::ServerTriggerInteraction_Implementation(int32 Index)
// {
// 	TriggerInteraction(Index, false);
// }


// bool UGGS_InteractionSystemComponent::ApplyGameplayBehaviorConfigToAbilitySystem(const FSmartObjectRequestResult& RequestResult, UAbilitySystemComponent* Asc,
//                                                                                  FGGS_InteractionInstance& InteractionInstance)
// {
// 	USmartObjectSubsystem* Subsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
//
// 	if (!Subsystem)
// 	{
// 		return false;
// 	}
// 	if (!IsValid(Asc))
// 	{
// 		return false;
// 	}
// 	UGameplayBehaviorConfig* Config = UGGS_SmartObjectFunctionLibrary::GetGameplayBehaviorConfig(
// 		Subsystem->GetBehaviorDefinitionByRequestResult(RequestResult, UGameplayBehaviorSmartObjectBehaviorDefinition::StaticClass()));
//
// 	const UGGS_GameplayBehaviorConfig_InteractionWithAbility* InteractionConfig = Cast<const UGGS_GameplayBehaviorConfig_InteractionWithAbility>(Config);
// 	if (!InteractionConfig)
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to find GameplayBehaviorConfig on RequestResult."), *Asc->GetAvatarActor()->GetName());
// 		return false;
// 	}
//
// 	const TSubclassOf<UGameplayAbility> AbilityClass = InteractionConfig->AbilityToGrant.LoadSynchronous();
// 	if (!AbilityClass)
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayBehaviorConfig:%s for option:%s due to AbilityToGrant being null"), *Asc->GetAvatarActor()->GetName(),
// 		       *InteractionConfig->GetClass()->GetName(), *InteractionInstance.Definition.Text.ToString());
// 		return false;
// 	}
//
// 	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayBehaviorConfig:%s for option:%s due to repeat adding ability class:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *InteractionConfig->GetClass()->GetName(), *InteractionInstance.Definition.Text.ToString(),
// 		       *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	// build and validate the ability spec
// 	FGameplayAbilitySpec AbilitySpec = Asc->BuildAbilitySpecFromClass(AbilityClass, InteractionConfig->AbilityLevel);
// 	// validate the class
// 	if (!IsValid(AbilitySpec.Ability))
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayBehaviorConfig:%s for option:%s due to invalid ability class:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *InteractionConfig->GetClass()->GetName(), *InteractionInstance.Definition.Text.ToString(), *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	FGameplayAbilitySpecHandle AbilitySpecHandle = Asc->GiveAbility(AbilitySpec);
// 	if (!AbilitySpecHandle.IsValid())
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayBehaviorConfig:%s for option:%s, can't active ability:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *InteractionConfig->GetClass()->GetName(),
// 		       *InteractionInstance.Definition.Text.ToString(), *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	InteractionInstance.TargetAbilitySystem = Asc;
// 	InteractionInstance.TargetInteractionAbilityHandle = AbilitySpecHandle;
// 	return true;
// }
//
// bool UGGS_InteractionSystemComponent::ApplyGameplayAbilitiesBehaviorDefinition(const FSmartObjectRequestResult& RequestResult, UAbilitySystemComponent* Asc,
//                                                                                FGGS_InteractionInstance& InteractionInstance)
// {
// 	USmartObjectSubsystem* Subsystem = GetWorld()->GetSubsystem<USmartObjectSubsystem>();
//
// 	if (!Subsystem)
// 	{
// 		return false;
// 	}
// 	if (!IsValid(Asc))
// 	{
// 		return false;
// 	}
//
// 	const UGGS_GameplayAbilitiesSmartObjectBehaviorDefinition* Definition = Cast<UGGS_GameplayAbilitiesSmartObjectBehaviorDefinition>(
// 		Subsystem->GetBehaviorDefinitionByRequestResult(RequestResult, UGGS_GameplayAbilitiesSmartObjectBehaviorDefinition::StaticClass()));
//
// 	if (!Definition)
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to find GameplayAbilitiesSmartObjectBehaviorDefinition on RequestResult."), *Asc->GetAvatarActor()->GetName());
// 		return false;
// 	}
//
// 	const TSubclassOf<UGameplayAbility> AbilityClass = Definition->AbilityToGrant.LoadSynchronous();
// 	if (!AbilityClass)
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayAbilitiesSmartObjectBehaviorDefinition:%s due to AbilityToGrant being null"), *Asc->GetAvatarActor()->GetName(),
// 		       *Definition->GetClass()->GetName());
// 		return false;
// 	}
//
// 	if (FGameplayAbilitySpec* Handle = Asc->FindAbilitySpecFromClass(AbilityClass))
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayAbilitiesSmartObjectBehaviorDefinition:%s due to repeat adding ability class:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *Definition->GetClass()->GetName(),
// 		       *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	// build and validate the ability spec
// 	FGameplayAbilitySpec AbilitySpec = Asc->BuildAbilitySpecFromClass(AbilityClass, Definition->AbilityLevel);
// 	// validate the class
// 	if (!IsValid(AbilitySpec.Ability))
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayAbilitiesSmartObjectBehaviorDefinition:%s due to invalid ability class:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *Definition->GetClass()->GetName(), *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	FGameplayAbilitySpecHandle AbilitySpecHandle = Asc->GiveAbility(AbilitySpec);
// 	if (!AbilitySpecHandle.IsValid())
// 	{
// 		UE_LOG(LogGGS_Interaction, Error, TEXT("%s Failed to apply GameplayAbilitiesSmartObjectBehaviorDefinition:%s, can't active ability:%s"), *Asc->GetAvatarActor()->GetName(),
// 		       *Definition->GetClass()->GetName(),
// 		       *AbilityClass->GetName());
// 		return false;
// 	}
//
// 	InteractionInstance.TargetAbilitySystem = Asc;
// 	InteractionInstance.TargetInteractionAbilityHandle = AbilitySpecHandle;
// 	return true;
// }

void UGGS_InteractionSystemComponent::OnInteractActorChanged(AActor* PreInteractActor)
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		if (PreInteractActor != nullptr && ClaimedHandle.IsValid())
		{
			StopInteraction();
		}
		RefreshOptionsForActor();
	}

	OnInteractionActorChangedEvent.Broadcast(PreInteractActor, InteractActor);
}

void UGGS_InteractionSystemComponent::OnSmartObjectEventCallback(const FSmartObjectEventData& EventData)
{
	check(InteractActor != nullptr);

	// skip it if it is not caused by me.
	if (ClaimedHandle.IsValid() && ClaimedHandle.SmartObjectHandle == EventData.SmartObjectHandle && ClaimedHandle.SlotHandle == EventData.SlotHandle)
	{
		// We only cares about release if we claimed it.
		if (EventData.Reason == ESmartObjectChangeReason::OnReleased)
		{
			RefreshOptionsForActor();
			return;
		}
		return;
	}

	for (int32 i = 0; i < InteractionInstances.Num(); i++)
	{
		const FGGS_InteractionInstance& Option = InteractionInstances[i];
		if (EventData.SmartObjectHandle == Option.RequestResult.SmartObjectHandle && EventData.SlotHandle == Option.RequestResult.SlotHandle)
		{
			if (EventData.Reason == ESmartObjectChangeReason::OnOccupied || EventData.Reason == ESmartObjectChangeReason::OnReleased || EventData.Reason == ESmartObjectChangeReason::OnClaimed)
			{
				RefreshOptionsForActor();
			}
		}
	}
}

void UGGS_InteractionSystemComponent::OnInteractionInstancesChanged()
{
	OnInteractionInstancesChangedEvent.Broadcast();
}

void UGGS_InteractionSystemComponent::OnInteractionStateChanged(bool bPrevState)
{
	OnInteractionStateChangedEvent.Broadcast(bInteracting);
}

void UGGS_InteractionSystemComponent::RefreshOptionsForActor()
{
	USmartObjectSubsystem* Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());

	if (!Subsystem)
	{
		return;
	}

	// getting new options for current interact actor.
	TArray<FGGS_InteractionInstance> NewOptions;
	{
		TArray<FSmartObjectRequestResult> Results;
		if (IsValid(InteractActor) && FindSmartObjectsInActor(InteractActor, Results))
		{
			for (int32 i = 0; i < Results.Num(); i++)
			{
				FGGS_InteractionInstance Option;
				if (UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSlot(this, Results[i].SlotHandle, Option.Definition))
				{
					Option.SlotState = Subsystem->GetSlotState(Results[i].SlotHandle);
					Option.RequestResult = Results[i];
					Option.SlotIndex = i;
					Option.BehaviorDefinition = Subsystem->GetBehaviorDefinitionByRequestResult(Results[i], USmartObjectBehaviorDefinition::StaticClass());
					NewOptions.Add(Option);
				}
			}
		}
	}


	// check any options changed.
	bool bOptionsChanged = false;
	{
		if (NewOptions.Num() == InteractionInstances.Num())
		{
			NewOptions.Sort();

			for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
			{
				const FGGS_InteractionInstance& NewOption = NewOptions[OptionIndex];
				const FGGS_InteractionInstance& CurrentOption = InteractionInstances[OptionIndex];

				if (NewOption != CurrentOption)
				{
					bOptionsChanged = true;
					break;
				}
			}
		}
		else
		{
			bOptionsChanged = true;
		}
	}

	if (bOptionsChanged)
	{
		UE_LOG(LogGGS_Interaction, Verbose, TEXT("%s Interaction options changed."), *GetOwner()->GetName());
		// unregister event callbacks for existing options.
		for (FGGS_InteractionInstance& Option : InteractionInstances)
		{
			if (Option.DelegateHandle.IsValid())
			{
				if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Option.RequestResult.SlotHandle))
				{
					OnEventDelegate->Remove(Option.DelegateHandle);
					Option.DelegateHandle.Reset();
				}
			}
		}

		InteractionInstances = NewOptions;

		// register slot event callbacks.
		for (int32 i = 0; i < InteractionInstances.Num(); i++)
		{
			FGGS_InteractionInstance& Option = InteractionInstances[i];
			if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Option.RequestResult.SlotHandle))
			{
				Option.DelegateHandle = OnEventDelegate->AddUObject(this, &ThisClass::OnSmartObjectEventCallback);
			}
		}

		OnInteractionInstancesChanged();
	}
}

void UGGS_InteractionSystemComponent::OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted)
{
	// Adding an ensure in case the assumptions change in the future.
	ensure(GetOwner() != nullptr);

	// make sure we handle the right pawn - we can get this notify for a different
	// Avatar if the behavior sending it out is not instanced (CDO is being used to perform actions)
	if (GetOwner() == &Avatar)
	{
		Behavior.GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		bBehaviorFinished = true;
		OnInteractionStateChangedEvent.Broadcast(false);
		StopInteraction();
	}
}

void UGGS_InteractionSystemComponent::OnClaimedSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State)
{
	//Todo, may happen if target destroyed.
	UE_LOG(LogGGS_Interaction, Warning, TEXT("%s"), State !=ESmartObjectSlotState::Free?TEXT("Non-Free"):TEXT("Free"));
}
