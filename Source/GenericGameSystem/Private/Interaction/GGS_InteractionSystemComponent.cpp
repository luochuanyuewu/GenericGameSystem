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
#include "Interaction/GGS_InteractableInterface.h"
#include "Interaction/Behaviors/GGS_GameplayBehaviorConfig_InteractionWithAbility.h"
#include "Interaction/GGS_InteractionDefinition.h"
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

void UGGS_InteractionSystemComponent::SearchInteractableActors()
{
	OnSearchInteractableActorsEvent.Broadcast();
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
		UGGS_InteractionDefinition* FoundDefinition;
		if (UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSlot(this, Result.SlotHandle, FoundDefinition))
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

void UGGS_InteractionSystemComponent::OnInteractActorChanged(AActor* PreInteractActor)
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		RefreshOptionsForActor();
	}

	if (IsValid(PreInteractActor) && PreInteractActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		IGGS_InteractableInterface::Execute_OnInteractionDeselected(PreInteractActor,GetOwner());
	}
	
	if (IsValid(InteractActor) && InteractActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		IGGS_InteractableInterface::Execute_OnInteractionSelected(InteractActor,GetOwner());
	}

	OnInteractionActorChangedEvent.Broadcast(PreInteractActor, InteractActor);
}

void UGGS_InteractionSystemComponent::OnSmartObjectEventCallback(const FSmartObjectEventData& EventData)
{
	check(InteractActor != nullptr);

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
	if (IsValid(InteractActor) && InteractActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		if (bInteracting)
		{
			IGGS_InteractableInterface::Execute_OnInteractionBegin(InteractActor,GetOwner());
		}else
		{
			IGGS_InteractableInterface::Execute_OnInteractionEnd(InteractActor,GetOwner());
		}
	}
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
				UGGS_InteractionDefinition* FoundDefinition;
				if (UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSlot(this, Results[i].SlotHandle, FoundDefinition))
				{
					Option.Definition = FoundDefinition;
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

		INTERACTION_RLOG(Verbose,TEXT("Interaction options changed, nums of options:%d"),InteractionInstances.Num())

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

