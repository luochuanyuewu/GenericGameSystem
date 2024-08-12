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

UCLASS(ClassGroup = (GES), hidecategories = (Variable, Tags, ComponentTick, ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta = (BlueprintSpawnableComponent))
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

public:
	// AnimMotionEffect Implementation
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	virtual void AnimMotionEffect_Implementation(const FName Bone, const FGameplayTag MotionEffect, USceneComponent *StaticMeshComponent,
												 const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase *AnimationSequence,
												 const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts,
												 FVector VFXScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1) override;

	// Auto-Convert Physical Surface from Trace Result to Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	bool bConvertPhysicalSurfaceToContext = true;

	// Default Contexts
	UPROPERTY(EditAnywhere, Category="Settings")
	FGameplayTagContainer DefaultEffectContexts;

	// Default Libraries for this Actor
	UPROPERTY(EditAnywhere, Category="Settings")
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> DefaultContextEffectsLibraries;

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void UpdateLibraries(TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> NewContextEffectsLibraries);

private:
	UPROPERTY(Transient)
	FGameplayTagContainer CurrentContexts;

	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> CurrentContextEffectsLibraries;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;
};
