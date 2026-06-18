// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGS_InteractionStructLibrary.h"
#include "SmartObjectSubsystem.h"
#include "Components/ActorComponent.h"
#include "GGS_InteractionSystemComponent.generated.h"

class UCommonUserWidget;
class UGameplayBehavior;

/**
 * Delegate for interaction events.
 * 交互事件的委托。
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionEventSignature);

/**
 * Delegate for changes in the interactable actor.
 * 可交互演员变更的委托。
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractableActorChangedSignature, AActor*, OldActor, AActor*, NewActor);

/**
 * Delegate for changes in the interacting state.
 * 交互状态变更的委托。
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractingStateChangedSignature, bool, bInteracting);

/**
 * Delegate for changes in the number of interactable actors.
 * 可交互演员数量变更的委托。
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableActorNumChangedSignature, int32, ActorsNum);

/**
 * Server-authoritative interaction coordinator for SmartObject-driven gameplay.
 * 基于 SmartObject 的服务端权威交互协调组件。
 * @details The server owns candidate selection and option refresh; owner clients receive replicated presentation state only.
 * @细节 服务端负责候选体选择与选项刷新；拥有者客户端只接收复制后的展示状态。
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(GGS), meta=(BlueprintSpawnableComponent))
class GENERICGAMESYSTEM_API UGGS_InteractionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for the interaction system component.
	 * 交互系统组件构造函数。
	 */
	UGGS_InteractionSystemComponent();

	/**
	 * Retrieves lifetime replicated properties.
	 * 获取生命周期内同步的属性。
	 * @param OutLifetimeProps The replicated properties. 同步的属性。
	 */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Retrieves the interaction system component from an actor.
	 * 从演员获取交互系统组件。
	 * @param Actor The actor to query. 要查询的演员。
	 * @return The interaction system component. 交互系统组件。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem", meta=(DefaultToSelf="Actor"))
	static UGGS_InteractionSystemComponent* GetInteractionSystemComponent(const AActor* Actor);

	/**
	 * Owner-requested target switching, executed on server to keep authority on selection.
	 * 拥有者请求切换目标，在服务端执行以保持目标选择的权威性。
	 * @param bNext Whether to cycle to the next actor. 是否切换到下一个演员。
	 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="GGS|InteractionSystem")
	void CycleInteractableActors(bool bNext);

	/**
	 * Broadcast-only search trigger; concrete trace/query strategy is expected in external listeners.
	 * 仅负责广播搜索触发；具体的检测/查询策略由外部监听方实现。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SearchInteractableActors();

	/**
	 * Replaces server-side candidate cache and drives owner-only replicated view state refresh.
	 * 替换服务端候选缓存，并触发面向拥有者复制的展示状态刷新。
	 * @param NewActors The new interactable actors. 新的可交互演员。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteractableActors(TArray<AActor*> NewActors);

	/**
	 * Sets the number of interactable actors.
	 * 设置可交互演员的数量。
	 * @param NewNum The new number of interactable actors. 可交互演员的新数量。
	 */
	void SetInteractableActorsNum(int32 NewNum);

	/**
	 * Retrieves the array of interactable actors.
	 * 获取可交互演员数组。
	 * @return The interactable actors. 可交互演员。
	 */
	TArray<AActor*> GetInteractableActors() const { return InteractableActors; }

	/**
	 * Retrieves the number of interactable actors.
	 * 获取可交互演员的数量。
	 * @return The number of interactable actors. 可交互演员数量。
	 */
	int32 GetNumOfInteractableActors() const { return NumsOfInteractableActors; }

	/**
	 * Updates current focus target; call path is expected to preserve authority ownership semantics.
	 * 更新当前焦点目标；调用方应保证符合权威端所有权语义。
	 * @param InActor The actor to set. 要设置的演员。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void SetInteractableActor(AActor* InActor);

	/**
	 * Retrieves the current interactable actor.
	 * 获取当前可交互演员。
	 * @return The interactable actor. 可交互演员。
	 */
	AActor* GetInteractableActor() const { return InteractableActor; }

	/**
	 * Delegate for when the interactable actor changes.
	 * 可交互演员变更时的委托。
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractableActorChangedSignature OnInteractableActorChangedEvent;

	/**
	 * Delegate for when the number of interactable actors changes.
	 * 可交互演员数量变更时的委托。
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractableActorNumChangedSignature OnInteractableActorNumChangedEvent;

	/**
	 * Delegate for when the interacting state changes.
	 * 交互状态变更时的委托。
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractingStateChangedSignature OnInteractingStateChangedEvent;

	/**
	 * Delegate for when the interaction options change.
	 * 交互选项变更时的委托。
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnInteractionOptionsChangedEvent;

	/**
	 * Delegate for when a search for interactable actors is triggered.
	 * 触发可交互演员搜索时的委托。
	 */
	UPROPERTY(BlueprintAssignable)
	FInteractionEventSignature OnSearchInteractableActorsEvent;

	/**
	 * Provides per-component query filter used by option refresh and actor search.
	 * 提供组件级查询过滤器，供选项刷新与候选搜索复用。
	 * @return The smart object request filter. 智能对象请求过滤器。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter GetSmartObjectRequestFilter();
	virtual FSmartObjectRequestFilter GetSmartObjectRequestFilter_Implementation();

	/**
	 * Starts persistent interaction state for a validated option index.
	 * 针对合法选项索引启动持续交互状态。
	 * @param NewIndex The interaction option index. 交互选项索引。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	virtual void StartInteraction(int32 NewIndex = 0);

	/**
	 * Clears persistent interaction state and emits transition callbacks.
	 * 清理持续交互状态并触发状态迁移回调。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	virtual void EndInteraction();

	/**
	 * Executes a one-shot interaction pulse (start then immediate end) for trigger-like actions.
	 * 执行一次性脉冲交互（开始后立即结束），用于触发式动作。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="GGS|InteractionSystem")
	void InstantInteraction(int32 NewIndex = 0);

	/**
	 * Checks if an interaction is in progress.
	 * 检查是否正在进行交互。
	 * @return True if interacting, false otherwise. 如果正在交互返回true，否则返回false。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	bool IsInteracting() const;

	/**
	 * Retrieves the current interacting option index.
	 * 获取当前交互选项索引。
	 * @return The interacting option index. 交互选项索引。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	int32 GetInteractingOption() const;

	/**
	 * Retrieves the current interaction options.
	 * 获取当前交互选项。
	 * @return The interaction options. 交互选项。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GGS|InteractionSystem")
	const TArray<FGGS_InteractionOption>& GetInteractionOptions() const { return InteractionOptions; }

protected:
	/**
	 * Called when the interactable actor changes.
	 * 可交互演员变更时调用。
	 * @param OldActor The previous interactable actor. 之前的可交互演员。
	 */
	UFUNCTION()
	virtual void OnInteractableActorChanged(AActor* OldActor);

	/**
	 * Called when the number of interactable actors changes.
	 * 可交互演员数量变更时调用。
	 */
	UFUNCTION()
	virtual void OnInteractableActorsNumChanged();

	/**
	 * Handles candidate list replacement and focus fallback logic after search/update.
	 * 处理搜索/更新后的候选列表替换与焦点回退逻辑。
	 */
	UFUNCTION(BlueprintNativeEvent, Category="GGS|InteractionSystem")
	void OnInteractableActorsChanged();

	/**
	 * Called when a smart object event occurs.
	 * 智能对象事件发生时调用。
	 * @param EventData The smart object event data. 智能对象事件数据。
	 */
	UFUNCTION()
	virtual void OnSmartObjectEventCallback(const FSmartObjectEventData& EventData);

	/**
	 * Called when interaction options change.
	 * 交互选项变更时调用。
	 */
	UFUNCTION()
	virtual void OnInteractionOptionsChanged();

	/**
	 * Called when the interacting option index changes.
	 * 交互选项索引变更时调用。
	 * @param PrevOptionIndex The previous option index. 之前的选项索引。
	 */
	UFUNCTION(BlueprintNativeEvent, Category="GGS|InteractionSystem")
	void OnInteractingOptionChanged(int32 PrevOptionIndex);

	/**
	 * Rebuilds option snapshot from slot query results, then rebinds slot callbacks atomically.
	 * 基于槽位查询结果重建选项快照，并原子化重绑槽位回调。
	 */
	virtual void RefreshOptionsForActor();

	/**
	 * Authority-side candidate cache; not replicated because only derived UI state is synced.
	 * 权威端候选缓存；不直接复制，仅同步其派生 UI 状态。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	TArray<TObjectPtr<AActor>> InteractableActors;

	/**
	 * Lightweight replicated count for owner-side UI hints without exposing full candidate list.
	 * 轻量复制的候选数量，用于拥有者 UI 提示且不暴露完整候选列表。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="GGS|InteractionSystem")
	int32 NumsOfInteractableActors{0};

	/**
	 * Current focused target replicated owner-only to drive local prompts/highlight.
	 * 当前焦点目标仅复制给拥有者，用于本地提示与高亮。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GGS|InteractionSystem", ReplicatedUsing=OnInteractableActorChanged)
	TObjectPtr<AActor> InteractableActor;

	/**
	 * Baseline SmartObject filter; per-pawn overrides can be provided via GetSmartObjectRequestFilter.
	 * SmartObject 基础过滤器；可通过 GetSmartObjectRequestFilter 提供角色级覆盖。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	FSmartObjectRequestFilter DefaultRequestFilter;

	/**
	 * When true, newest candidate ordering can preempt current focus to keep "nearest/highest-priority first" UX.
	 * 为 true 时，新候选排序可抢占当前焦点，以维持“最近/最高优先级优先”的交互体验。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnInteractableActorsNumChanged, Category="GGS|InteractionSystem")
	bool bNewActorHasPriority{false};

	/**
	 * Owner-facing option snapshot derived from authoritative slot query and slot state filtering.
	 * 面向拥有者的选项快照，由权威端槽位查询与槽位状态过滤结果派生。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractionOptionsChanged)
	TArray<FGGS_InteractionOption> InteractionOptions;

	/**
	 * Local runtime flag derived from InteractingOption transitions for fast branch checks.
	 * 由 InteractingOption 迁移推导的本地运行时标记，用于快速分支判断。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem")
	bool bInteracting{false};

	/**
	 * Replicated interaction cursor; INDEX_NONE means "no active interaction".
	 * 复制的交互游标；INDEX_NONE 表示“无进行中交互”。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GGS|InteractionSystem", ReplicatedUsing=OnInteractingOptionChanged)
	int32 InteractingOption{INDEX_NONE};

	/**
	 * Slot-event subscription registry used to safely unsubscribe stale callbacks during option rebuild.
	 * 槽位事件订阅表，用于在选项重建时安全解绑陈旧回调。
	 */
	TMap<FSmartObjectSlotHandle, FDelegateHandle> SlotCallbacks;
};
