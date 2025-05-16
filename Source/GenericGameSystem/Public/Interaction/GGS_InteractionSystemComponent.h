// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGS_InteractionStructLibrary.h"
#include "SmartObjectSubsystem.h"
#include "Components/ActorComponent.h"
#include "GGS_InteractionSystemComponent.generated.h"

class UCommonUserWidget;
class UGameplayBehavior;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionEventSignature);

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

	/**
	 * Force a search. 
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SearchInteractableActors();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetPotentialActors(TArray<AActor*> NewActors);

	//Set the currently interacting target actor
	void SetInteractActor(AActor* InActor);

	UPROPERTY(BlueprintAssignable)
	FInteractionActorChangedSignature OnInteractionActorChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractionStateChangedSignature OnInteractionStateChangedEvent;

	//Called when intaction list changed.
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnInteractionInstancesChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnSearchInteractableActorsEvent;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter GetSmartObjectRequestFilter();
	virtual FSmartObjectRequestFilter GetSmartObjectRequestFilter_Implementation();

	bool FindSmartObjectsInActor(AActor* InActor, TArray<FSmartObjectRequestResult>& OutResults);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteractionState(bool bNewState);

	/**
	 * @return Return currently if any active interacting. 当前是否有交互?
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	bool GetInteractionState() const;

	/**
	 * @return Currently available interaction options. 当前可用交互选项.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	const TArray<FGGS_InteractionInstance>& GetInteractionInstances() const { return InteractionInstances; };


protected:
	UFUNCTION()
	virtual void OnInteractActorChanged(AActor* PreInteractActor);

	UFUNCTION()
	virtual void OnSmartObjectEventCallback(const FSmartObjectEventData& EventData);

	UFUNCTION()
	virtual void OnInteractionInstancesChanged();

	UFUNCTION()
	virtual void OnInteractionStateChanged(bool bPrevState);

	/**
	 * create new interaction instances  based on new smart object request results. 
	 */
	virtual void RefreshOptionsForActor();

	/**
	 * Searched actor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	TArray<TObjectPtr<AActor>> PotentialActors;
	
	/** 玩家交互的Actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GGS|InteractionSystem", ReplicatedUsing=OnInteractActorChanged)
	TObjectPtr<AActor> InteractActor;

	/**
	 * 配置搜索交互对象的过滤器。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;

	UPROPERTY(VisibleAnywhere, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionInstancesChanged)
	TArray<FGGS_InteractionInstance> InteractionInstances;

	/**
	 * Indicating if there's any interaction in progress.
	 */
	UPROPERTY(VisibleAnywhere, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionStateChanged)
	bool bInteracting{false};

};
