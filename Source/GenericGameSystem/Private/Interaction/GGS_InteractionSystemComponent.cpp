// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_InteractionSystemComponent.h"
#include "Engine/World.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GGS_LogChannels.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"
#include "Interaction/GGS_InteractableInterface.h"
#include "Interaction/GGS_SmartObjectFunctionLibrary.h"
#include "Interaction/GGS_InteractionStructLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values for this component's properties
UGGS_InteractionSystemComponent::UGGS_InteractionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UGGS_InteractionSystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InteractableActor, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, NumsOfInteractableActors, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bInteracting, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, InteractionOptions, Params);
}

UGGS_InteractionSystemComponent* UGGS_InteractionSystemComponent::GetInteractionSystemComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UGGS_InteractionSystemComponent>() : nullptr;
}

void UGGS_InteractionSystemComponent::CycleInteractableActors_Implementation(bool bNext)
{
	if (bInteracting || InteractableActors.Num() <= 1)
	{
		return;
	}

	int32 Index = InteractableActor != nullptr ? InteractableActors.IndexOfByKey(InteractableActor) : 0;
	if (!InteractableActors.IsValidIndex(Index)) //一个都没
	{
		return;
	}
	if (bNext)
	{
		Index = FMath::Clamp(Index + 1, 0, InteractableActors.Num());
	}
	else
	{
		Index = FMath::Clamp(Index - 1, 0, InteractableActors.Num());
	}
	if (InteractableActors.IsValidIndex(Index) && InteractableActors[Index] != nullptr && InteractableActors[Index] !=
		InteractableActor)
	{
		SetInteractableActor(InteractableActors[Index]);
	}
}

void UGGS_InteractionSystemComponent::SearchInteractableActors()
{
	OnSearchInteractableActorsEvent.Broadcast();
}

void UGGS_InteractionSystemComponent::SetInteractableActors(TArray<AActor*> NewActors)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	InteractableActors = NewActors;
	SetInteractableActorsNum(InteractableActors.Num());

	if (!bInteracting)
	{
		// update potential actor.
		if (!IsValid(InteractableActor) || !InteractableActors.Contains(InteractableActor))
		{
			if (NewActors.IsValidIndex(0) && IsValid(NewActors[0]))
			{
				SetInteractableActor(NewActors[0]);
			}
			else
			{
				SetInteractableActor(nullptr);
			}
		}
	}
}

void UGGS_InteractionSystemComponent::SetInteractableActorsNum(int32 NewNum)
{
	if (NewNum != NumsOfInteractableActors)
	{
		const int32 PrevNum = NumsOfInteractableActors;
		NumsOfInteractableActors = NewNum;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, NumsOfInteractableActors, this)
		OnInteractableActorsNumChanged(PrevNum);
	}
}

void UGGS_InteractionSystemComponent::SetInteractableActor(AActor* InActor)
{
	if (InActor != InteractableActor)
	{
		AActor* OldActor = InteractableActor;
		InteractableActor = InActor;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractableActor, this)
		OnInteractableActorChanged(OldActor);
	}
}

FSmartObjectRequestFilter UGGS_InteractionSystemComponent::GetSmartObjectRequestFilter_Implementation()
{
	return DefaultRequestFilter;
}

void UGGS_InteractionSystemComponent::SetInteracting(bool bNewState)
{
	if (bInteracting != bNewState)
	{
		bool prevState = bInteracting;
		bInteracting = bNewState;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, bInteracting, this);
		OnInteractingStateChanged(prevState);
	}
}

bool UGGS_InteractionSystemComponent::IsInteracting() const
{
	return bInteracting;
}

void UGGS_InteractionSystemComponent::OnInteractableActorChanged(AActor* OldActor)
{
	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		RefreshOptionsForActor();
	}

	if (IsValid(OldActor) && OldActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		IGGS_InteractableInterface::Execute_OnInteractionDeselected(OldActor, GetOwner());
	}

	if (IsValid(InteractableActor) && InteractableActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		IGGS_InteractableInterface::Execute_OnInteractionSelected(InteractableActor, GetOwner());
	}

	OnInteractableActorChangedEvent.Broadcast(OldActor, InteractableActor);
}

void UGGS_InteractionSystemComponent::OnInteractableActorsNumChanged(int32 PrevNum)
{
	OnInteractableActorNumChangedEvent.Broadcast(NumsOfInteractableActors);
}

void UGGS_InteractionSystemComponent::OnSmartObjectEventCallback(const FSmartObjectEventData& EventData)
{
	check(InteractableActor != nullptr);

	for (int32 i = 0; i < InteractionOptions.Num(); i++)
	{
		const FGGS_InteractionOption& Option = InteractionOptions[i];
		if (EventData.SmartObjectHandle == Option.RequestResult.SmartObjectHandle && EventData.SlotHandle == Option.RequestResult.SlotHandle)
		{
			if (EventData.Reason == ESmartObjectChangeReason::OnOccupied || EventData.Reason == ESmartObjectChangeReason::OnReleased || EventData.Reason == ESmartObjectChangeReason::OnClaimed)
			{
				RefreshOptionsForActor();
			}
		}
	}
}

void UGGS_InteractionSystemComponent::OnInteractionOptionsChanged()
{
	for (FGGS_InteractionOption& InteractionOption : InteractionOptions)
	{
		INTERACTION_RLOG(Verbose, TEXT("Available Options:%s"), *InteractionOption.ToString())
	}
	OnInteractionOptionsChangedEvent.Broadcast();
}

void UGGS_InteractionSystemComponent::OnInteractingStateChanged(bool bPrevState)
{
	if (IsValid(InteractableActor) && InteractableActor->GetClass()->ImplementsInterface(UGGS_InteractableInterface::StaticClass()))
	{
		if (!bPrevState && bInteracting)
		{
			IGGS_InteractableInterface::Execute_OnInteractionStarted(InteractableActor, GetOwner());
		}
		if (bPrevState && !bInteracting)
		{
			IGGS_InteractableInterface::Execute_OnInteractionEnded(InteractableActor, GetOwner());
		}
	}
	OnInteractingStateChangedEvent.Broadcast(bInteracting);
}

void UGGS_InteractionSystemComponent::RefreshOptionsForActor()
{
	USmartObjectSubsystem* Subsystem = USmartObjectSubsystem::GetCurrent(GetWorld());

	if (!Subsystem)
	{
		return;
	}

	// getting new options for current interact actor.
	TArray<FGGS_InteractionOption> NewOptions;
	{
		TArray<FSmartObjectRequestResult> Results;
		if (IsValid(InteractableActor) && UGGS_SmartObjectFunctionLibrary::FindSmartObjectsWithInteractionEntranceInActor(GetSmartObjectRequestFilter(), InteractableActor, Results, GetOwner()))
		{
			for (int32 i = 0; i < Results.Num(); i++)
			{
				FGGS_InteractionOption Option;
				UGGS_InteractionDefinition* FoundDefinition;
				if (UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSmartObjectSlot(this, Results[i].SlotHandle, FoundDefinition))
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
		if (NewOptions.Num() == InteractionOptions.Num())
		{
			NewOptions.Sort();

			for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
			{
				const FGGS_InteractionOption& NewOption = NewOptions[OptionIndex];
				const FGGS_InteractionOption& CurrentOption = InteractionOptions[OptionIndex];

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
		for (int32 i = 0; i < InteractionOptions.Num(); i++)
		{
			auto& Handle = InteractionOptions[i].RequestResult.SlotHandle;
			if (SlotCallbacks.Contains(Handle))
			{
				if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Handle))
				{
					OnEventDelegate->Remove(SlotCallbacks[Handle]);
					SlotCallbacks.Remove(Handle);
				}
			}
		}

		for (FGGS_InteractionOption& Option : InteractionOptions)
		{
			if (SlotCallbacks.Contains(Option.RequestResult.SlotHandle))
			{
				if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Option.RequestResult.SlotHandle))
				{
					OnEventDelegate->Remove(SlotCallbacks[Option.RequestResult.SlotHandle]);
					SlotCallbacks.Remove(Option.RequestResult.SlotHandle);
				}
			}
			// if (Option.DelegateHandle.IsValid())
			// {
			// 	if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Option.RequestResult.SlotHandle))
			// 	{
			// 		OnEventDelegate->Remove(Option.DelegateHandle);
			// 		Option.DelegateHandle.Reset();
			// 	}
			// }
		}

		InteractionOptions = NewOptions;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, InteractionOptions, this)

		INTERACTION_RLOG(Verbose, TEXT("Interaction options changed, nums of options:%d"), InteractionOptions.Num())

		// register slot event callbacks.
		// for (int32 i = 0; i < InteractionOptions.Num(); i++)
		// {
		// 	FGGS_InteractionOption& Option = InteractionOptions[i];
		// 	if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Option.RequestResult.SlotHandle))
		// 	{
		// 		Option.DelegateHandle = OnEventDelegate->AddUObject(this, &ThisClass::OnSmartObjectEventCallback);
		// 	}
		// }

		for (int32 i = 0; i < InteractionOptions.Num(); i++)
		{
			auto& Handle = InteractionOptions[i].RequestResult.SlotHandle;
			if (FOnSmartObjectEvent* OnEventDelegate = Subsystem->GetSlotEventDelegate(Handle))
			{
				FDelegateHandle DelegateHandle = OnEventDelegate->AddUObject(this, &ThisClass::OnSmartObjectEventCallback);
				SlotCallbacks.Emplace(Handle, DelegateHandle);
			}
		}

		OnInteractionOptionsChanged();
	}
}
