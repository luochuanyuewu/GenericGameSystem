// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGS_InteractionStructLibrary.h"
#include "SmartObjectSubsystem.h"
#include "Components/ActorComponent.h"
#include "GGS_InteractionSystemComponent.generated.h"

class UCommonUserWidget;
class UGameplayBehavior;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionInstancesChangedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractionActorChangedSignature, AActor*, OldInteractActor, AActor*, InteractActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionStateChangedSignature, bool, bInteracting);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPotentialActorsNumChangedSignature, int32, ActorsNum);


UCLASS(Blueprintable, BlueprintType, ClassGroup=(GGS), meta=(BlueprintSpawnableComponent))
class GENERICGAMESYSTEM_API UGGS_InteractionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGGS_InteractionSystemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem", meta=(DefaultToSelf="Actor"))
	static UGGS_InteractionSystemComponent* GetInteractionSystemComponent(const AActor* Actor);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="GGS|InteractionSystem")
	void CycleInteractActors(bool bNext);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetPotentialActors(TArray<AActor*> NewActors);

	//Set the currently interacting target actor
	void SetInteractActor(AActor* InActor);

	UPROPERTY(BlueprintAssignable)
	FInteractionActorChangedSignature OnInteractionActorChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractionStateChangedSignature OnInteractionStateChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FPotentialActorsNumChangedSignature OnPotentialActorsNumChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractionInstancesChangedSignature OnInteractionInstancesChangedEvent;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter GetSmartObjectRequestFilter();
	virtual FSmartObjectRequestFilter GetSmartObjectRequestFilter_Implementation();

	bool FindSmartObjectsInActor(AActor* InActor, TArray<FSmartObjectRequestResult>& OutResults);

	void SetInteractionState(bool bNewState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	bool GetInteractionState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	TArray<FGGS_InteractionInstance> GetInteractionInstances() const { return InteractionInstances; };

	// UFUNCTION(BlueprintCallable, Category="GGS|InteractionSystem")
	// virtual void TriggerInteraction(int32 Index);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="GGS|InteractionSystem")
	void StartInteraction(int32 Index);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="GGS|InteractionSystem")
	void StopInteraction();

protected:

	//Claim and Ocupate smart object, then trigger gameplay behavior.
	bool InternalStartInteraction(int32 Index);
	
	// void TriggerInteraction(int32 Index, bool bSendRpc);

	// UFUNCTION(Server, Reliable)
	// void ServerTriggerInteraction(int32 Index);
	
	// bool ApplyGameplayBehaviorConfigToAbilitySystem(const FSmartObjectRequestResult& RequestResult,UAbilitySystemComponent* Asc,FGGS_InteractionInstance& InteractionInstance);
	//
	// bool ApplyGameplayAbilitiesBehaviorDefinition(const FSmartObjectRequestResult& RequestResult,UAbilitySystemComponent* Asc,FGGS_InteractionInstance& InteractionInstance);

protected:
	UFUNCTION()
	virtual void OnInteractActorChanged(AActor* PreInteractActor);

	UFUNCTION()
	virtual void OnSmartObjectEventCallback(const FSmartObjectEventData& EventData);

	UFUNCTION()
	virtual void OnInteractionInstancesChanged();

	UFUNCTION()
	virtual void OnPotentialActorsNumChanged();

	UFUNCTION()
	virtual void OnInteractionStateChanged(bool bPreState);

	/**
	 * create new interaction instances  based on new smart object request results. 
	 */
	virtual void RefreshOptionsForActor();

	void OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted);

	void OnClaimedSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	TArray<TObjectPtr<AActor>> PotentialActors;

	/** 玩家交互的Actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractActorChanged)
	TObjectPtr<AActor> InteractActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnPotentialActorsNumChanged)
	int32 PotentialActorsNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;

	UPROPERTY(VisibleAnywhere, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionInstancesChanged)
	TArray<FGGS_InteractionInstance> InteractionInstances;

	UPROPERTY()
	TObjectPtr<UGameplayBehavior> GameplayBehavior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	FSmartObjectClaimHandle ClaimedHandle;

	/**
	 * Indicating if there's any interaction in progress.
	 */
	UPROPERTY(VisibleAnywhere, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionStateChanged)
	bool bInteracting{false};

	FDelegateHandle OnBehaviorFinishedNotifyHandle;

	bool bBehaviorFinished;
};
