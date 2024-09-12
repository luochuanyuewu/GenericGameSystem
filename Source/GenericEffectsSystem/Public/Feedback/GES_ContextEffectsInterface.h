// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/HitResult.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "GES_ContextEffectsInterface.generated.h"

class UAnimSequenceBase;
class UObject;
class USceneComponent;
struct FFrame;

/**
 *
 */
UENUM()
enum EGES_EffectsContextMatchType : int
{
	ExactMatch,
	BestMatch
};

/**
 * Any Object implement this interface can responses to context effects.
 */
UINTERFACE(Blueprintable)
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class GENERICEFFECTSSYSTEM_API IGES_ContextEffectsInterface : public IInterface
{
	GENERATED_BODY()

public:
	/** */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GES|ContextEffect",meta=(DeprecatedFunction,DeprecationMessage="Using Play Context Effects Attached"))
	void AnimMotionEffect(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent, const FVector LocationOffset, const FRotator RotationOffset,
	                      const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts, FVector VFXScale = FVector(1),
	                      float AudioVolume = 1, float AudioPitch = 1);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GES|ContextEffect")
	void PlayContextEffectsAttached(const FName Bone, const FGameplayTag EffectName, USceneComponent* ComponentToAttach, const FVector LocationOffset, const FRotator RotationOffset,const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts, FVector VFXScale = FVector(1),
						  float AudioVolume = 1, float AudioPitch = 1);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GES|ContextEffect")
	void PlayContextEffects(const FGameplayTag EffectName, const FVector Location, const FRotator Rotation,
											 const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts,
											 FVector VFXScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1);
};
