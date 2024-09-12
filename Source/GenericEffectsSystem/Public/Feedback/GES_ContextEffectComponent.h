// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

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
 * Default component which implements GES_ContextEffectsInterface to response to context effects. 
 */
UCLASS(ClassGroup = (GES), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories,
	meta = (BlueprintSpawnableComponent))
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectComponent : public UActorComponent, public IGES_ContextEffectsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGES_ContextEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupTagsProvider();

public:
	// AnimMotionEffect Implementation
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect",meta=(DisplayName="Play Context Effects(Attached)"))
	virtual void AnimMotionEffect_Implementation(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent,
	                                             const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase* AnimationSequence,
	                                             const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts,
	                                             FVector VFXScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1) override;

	virtual void PlayContextEffectsAttached_Implementation(const FName Bone, const FGameplayTag EffectName, USceneComponent* ComponentToAttach, const FVector LocationOffset,
		const FRotator RotationOffset,const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts, FVector VFXScale, float AudioVolume, float AudioPitch) override;

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect",meta=(DisplayName="Play Context Effects"))
	virtual void PlayContextEffects_Implementation(const FGameplayTag EffectName, const FVector Location, const FRotator Rotation,
												 const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts,
												 FVector VFXScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1) override;

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void SetGameplayTagsProvider(UObject* Provider);

	// Auto-Convert Physical Surface from Trace Result to Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	bool bConvertPhysicalSurfaceToContext = true;

	/**
	 * Fallback surface type when there are no mapping, or valid physical material.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings", meta=(Categories="GES.SurfaceType"))
	FGameplayTag FallbackPhysicalSurface;

	// Default Contexts
	UPROPERTY(EditAnywhere, Category="Settings")
	FGameplayTagContainer DefaultEffectContexts;

	// Default Libraries for this Actor
	UPROPERTY(EditAnywhere, Category="Settings")
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> DefaultContextEffectsLibraries;

	/**
	 * Adding additional gameplay tags to current contexts(will override default contexts)
	 */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateLibraries(TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> NewContextEffectsLibraries);

private:
	UPROPERTY(VisibleInstanceOnly, Transient, Category="State")
	FGameplayTagContainer CurrentContexts;

	/**
	 * If checked, will look for owner or owner's components to find one which implements GameplayTagAssetInterface, and set it as TagsProvider.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	bool bAutoSetupTagsProvider{false};

	/**
	 * This is an optional UObject which implements GameplayTagAssetInterface.
	 * The owned tags of this object will be the return value of GetGameplayTags;
	 * If you are using GameplayAbilitySystem, You can set ability system component as TagsProvider, so you can use AbilitySystemComponent's tags to drive EfectContexts.
	 */
	UPROPERTY(VisibleAnywhere, Category="State")
	TObjectPtr<UObject> GameplayTagsProvider{nullptr};

	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> CurrentContextEffectsLibraries;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;
};
