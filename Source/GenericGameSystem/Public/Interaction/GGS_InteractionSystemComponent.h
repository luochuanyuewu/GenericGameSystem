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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractableActorChangedSignature, AActor*, OldActor, AActor*, NewActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractingStateChangedSignature, bool, bInteracting);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableActorNumChangedSignature, int32, ActorsNum);


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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="GGS|InteractionSystem")
	void CycleInteractableActors(bool bNext);

	/**
	 * Force a potential interactable actors.
	 * 强制触发一次潜在可交互Actors的搜索。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SearchInteractableActors();

	/**
	 * Replace a new set of potential interactable actors.
	 * 替换潜在可交互Actors.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteractableActors(TArray<AActor*> NewActors);

	void SetInteractableActorsNum(int32 NewNum);

	TArray<AActor*> GetInteractableActors() const { return InteractableActors; }

	int32 GetNumOfInteractableActors() const { return NumsOfInteractableActors; }

	/**
	 * Set the actor you want to interact with.
	 * @param InActor The actor you want to interact with.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteractableActor(AActor* InActor);

	AActor* GetInteractableActor() const { return InteractableActor; }

	UPROPERTY(BlueprintAssignable)
	FInteractableActorChangedSignature OnInteractableActorChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractableActorNumChangedSignature OnInteractableActorNumChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractingStateChangedSignature OnInteractingStateChangedEvent;

	//Called when intaction list changed.
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnInteractionOptionsChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnSearchInteractableActorsEvent;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter GetSmartObjectRequestFilter();
	virtual FSmartObjectRequestFilter GetSmartObjectRequestFilter_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteracting(bool bNewState);

	/**
	 * @return Return currently if any active interacting. 当前是否有交互?
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	bool IsInteracting() const;

	/**
	 * @return Currently available interaction options. 当前可用交互选项.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	const TArray<FGGS_InteractionOption>& GetInteractionOptions() const { return InteractionOptions; }

protected:
	UFUNCTION()
	virtual void OnInteractableActorChanged(AActor* OldActor);

	UFUNCTION()
	virtual void OnInteractableActorsNumChanged(int32 ActorsNum);

	UFUNCTION()
	virtual void OnSmartObjectEventCallback(const FSmartObjectEventData& EventData);

	UFUNCTION()
	virtual void OnInteractionOptionsChanged();

	UFUNCTION()
	virtual void OnInteractingStateChanged(bool bPrevState);

	/**
	 * create new interaction instances  based on new smart object request results. 
	 */
	virtual void RefreshOptionsForActor();

	/**
	 * Potential interactable Actors, you can cycle between these actors to select the one you want to interact with.
	 * @attention This is not replicated.
	 * 潜在可交互Actors，你可以在它们之间切换来选择你想与之交互的Actor。
	 * @注意 未网络同步。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	TArray<TObjectPtr<AActor>> InteractableActors;

	/**
	 * Num of potential interactable actors, replicated to owning client.
	 * 潜在可交互Actors数量，同步到Owning客户端。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="GGS|InteractionSystem")
	int32 NumsOfInteractableActors{0};

	/**
	 * The current selected interactable actor. will build interaction options around this one.
	 * @attention This is network replicated for onwer only.
	 * 当前选择的可交互Actor。会基于该Actor构建交互选项。
	 * @注意 只针对Owner进行网络同步。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GGS|InteractionSystem", ReplicatedUsing=OnInteractableActorChanged)
	TObjectPtr<AActor> InteractableActor;

	/**
	 * Controls what filters will apply when you are looking for interactable smart objects.
	 * Subclass can override GetSmartObjectRequestFilter to provide dynamical filters.
	 * 控制当你搜索可交互只能对象时应用的过滤器。
	 * 子类可以覆写GetSmartObjectRequestFilter以提供动态的Filter。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;

	/**
	 * Current available interaction options, replicated for owner only.
	 * 当前可用的交互选择，仅针对Owner同步。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionOptionsChanged)
	TArray<FGGS_InteractionOption> InteractionOptions;

	/**
	 * Indicating if there's any interaction in progress.
	 * 表示是否有任何交互进行中。。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractingStateChanged)
	bool bInteracting{false};
};
