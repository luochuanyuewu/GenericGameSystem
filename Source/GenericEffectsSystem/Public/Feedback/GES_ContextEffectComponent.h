// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "GES_ContextEffectsInterface.h"
#include "GES_ContextEffectComponent.generated.h"

namespace EEndPlayReason
{
	enum Type : int;
}

class UAnimSequenceBase;
class UAudioComponent;
class UGES_ContextEffectsLibrary;
class UNiagaraComponent;
class UObject;
class USceneComponent;
struct FFrame;
struct FHitResult;

/**
 * Actor-level context effect bridge that resolves gameplay tags into data-driven VFX/SFX playback.
 * Actor 级情景特效桥接组件：将 GameplayTag 解析为数据驱动的 VFX/SFX 播放。
 * @details Runtime context is composed from input, component state, and optional tag provider before dispatching to subsystem.
 * @细节 运行时上下文由输入、组件状态和可选标签提供者共同构成，随后交给子系统执行。
 */
UCLASS(ClassGroup = (GES), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories,
	meta = (BlueprintSpawnableComponent))
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectComponent : public UActorComponent, public IGES_ContextEffectsInterface
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for the context effect component.
	 * 情景效果组件构造函数。
	 */
	UGES_ContextEffectComponent();

protected:
	/**
	 * Called when the game starts.
	 * 游戏开始时调用。
	 */
	virtual void BeginPlay() override;

	/**
	 * Called when the game ends.
	 * 游戏结束时调用。
	 * @param EndPlayReason The reason for ending play. 结束播放的原因。
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Sets up the gameplay tags provider.
	 * 设置游戏标签提供者。
	 */
	virtual void SetupTagsProvider();

public:
	/**
	 * Plays context effects based on input parameters.
	 * 根据输入参数播放情景效果。
	 * @param Input The context effects input data. 情景效果输入数据。
	 */
	virtual void PlayContextEffectsWithInput_Implementation(FGES_SpawnContextEffectsInput Input) override;

	/**
	 * Merges input context with component/provider context according to SourceContextType contract.
	 * 按 SourceContextType 契约合并输入上下文与组件/提供者上下文。
	 * @param Input The context effects input data. 情景效果输入数据。
	 */
	void AggregateContexts(FGES_SpawnContextEffectsInput& Input) const;

	/**
	 * Translates hit physical surface into context tags with fallback behavior from project settings.
	 * 将命中物理表面翻译为上下文标签，并在配置缺失时使用回退策略。
	 * @param InHitResult The hit result. 命中结果。
	 * @param Contexts The context tags (output). 情景标签（输出）。
	 */
	void InjectPhysicalSurfaceToContexts(const FHitResult& InHitResult, FGameplayTagContainer& Contexts);

	/**
	 * Sets the gameplay tags provider.
	 * 设置游戏标签提供者。
	 * @param Provider The object providing gameplay tags. 提供游戏标签的对象。
	 */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void SetGameplayTagsProvider(UObject* Provider);

	/**
	 * Automatically converts physical surface to context tags.
	 * 自动将物理表面转换为情景标签。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	bool bConvertPhysicalSurfaceToContext = true;

	/**
	 * Fallback surface type when no mapping or valid physical material exists.
	 * 当无映射或有效物理材料时使用的备用表面类型。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(Categories="GES.SurfaceType"))
	FGameplayTag FallbackPhysicalSurface;

	/**
	 * Default context tags for effect playback.
	 * 用于效果播放的默认情景标签。
	 */
	UPROPERTY(EditAnywhere, Category="Settings")
	FGameplayTagContainer DefaultEffectContexts;

	/**
	 * Startup library set loaded into subsystem for this owner; hot-swappable at runtime via UpdateLibraries.
	 * 该拥有者启动时加载到子系统的库集合；可通过 UpdateLibraries 在运行时热切换。
	 */
	UPROPERTY(EditAnywhere, Category="Settings")
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> DefaultContextEffectsLibraries;

	/**
	 * Updates the current effect contexts, overriding default contexts.
	 * 更新当前效果情景，覆盖默认情景。
	 * @param NewEffectContexts The new context tags. 新情景标签。
	 */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	/**
	 * Updates the context effects libraries.
	 * 更新情景效果库。
	 * @param NewContextEffectsLibraries The new effects libraries. 新效果库。
	 */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateLibraries(TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> NewContextEffectsLibraries);

private:
	/**
	 * Current context tags for effect playback.
	 * 用于效果播放的当前情景标签。
	 */
	UPROPERTY(VisibleInstanceOnly, Transient, Category="State")
	FGameplayTagContainer CurrentContexts;

	/**
	 * Automatically sets up the tags provider if enabled.
	 * 如果启用，自动设置标签提供者。
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	bool bAutoSetupTagsProvider{true};

	/**
	 * Optional object implementing GameplayTagAssetInterface for tag provision.
	 * 实现GameplayTagAssetInterface的可选对象，用于提供标签。
	 */
	UPROPERTY(VisibleAnywhere, Category="State")
	TObjectPtr<UObject> GameplayTagsProvider{nullptr};

	/**
	 * Runtime library snapshot currently registered for this owner in subsystem.
	 * 当前在子系统中为该拥有者注册的运行时库快照。
	 */
	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> CurrentContextEffectsLibraries;

	/**
	 * Active audio components for effect playback.
	 * 用于效果播放的活跃音频组件。
	 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	/**
	 * Active Niagara components for effect playback.
	 * 用于效果播放的活跃Niagara组件。
	 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;

	/**
	 * Active particle system components for effect playback.
	 * 用于效果播放的活跃粒子系统组件。
	 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UParticleSystemComponent>> ActiveParticleComponents;
};
