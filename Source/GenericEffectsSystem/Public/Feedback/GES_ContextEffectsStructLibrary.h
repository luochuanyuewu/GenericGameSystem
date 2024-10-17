// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GES_ContextEffectsStructLibrary.generated.h"

class UNiagaraComponent;
class UAudioComponent;
class UParticleSystemComponent;

/**
 * The definition of a context effects.
 */
USTRUCT(BlueprintType, DisplayName="GES Context Effects Definition")
struct GENERICEFFECTSSYSTEM_API FGES_ContextEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES", meta=(DisplayName="Source Contexts"))
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES", meta=(DisplayName="Target Contexts"))
	FGameplayTagContainer TargetContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES", meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem, /Script/Engine.ParticleSystem"))
	TArray<FSoftObjectPath> Effects;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category="GES", meta=(EditCondition=false, EditConditionHides))
	FString EditorFriendlyName;
#endif
};


/**
 *
 */
USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_ContextEffectAnimNotifyVFXSettings
{
	GENERATED_BODY()

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_ContextEffectAnimNotifyAudioSettings
{
	GENERATED_BODY()

	// Volume Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
	float VolumeMultiplier = 1.0f;

	// Pitch Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
	float PitchMultiplier = 1.0f;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_ContextEffectAnimNotifyTraceSettings
{
	GENERATED_BODY()

	// Trace Channel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	// Vector offset from Effect Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
	FVector EndTraceLocationOffset = FVector::ZeroVector;

	// Ignore this Actor when getting trace result
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Trace)
	bool bIgnoreActor = true;
};


USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_SpawnContextEffectsInput
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	FGameplayTag EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	bool bAttached{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES", meta=(EditCondition="!bAttached", EditConditionHides))
	FVector Location{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES", meta=(EditCondition="!bAttached", EditConditionHides))
	FRotator Rotation{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Context")
	FGameplayTagContainer SourceContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Context")
	FGameplayTagContainer TargetContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Attachment", meta=(EditCondition="bAttached", EditConditionHides))
	FName Bone{NAME_None};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Attachment", meta=(EditCondition="bAttached", EditConditionHides))
	USceneComponent* ComponentToAttach{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Attachment", meta=(EditCondition="bAttached", EditConditionHides))
	FVector LocationOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Attachment", meta=(EditCondition="bAttached", EditConditionHides))
	FRotator RotationOffset{ForceInit};

	/**
	 * Optional source animation which triggered this context effects.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	const UAnimSequenceBase* AnimationSequence{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Vfx")
	FVector VFXScale = FVector(1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Sfx")
	float AudioVolume = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Sfx")
	float AudioPitch = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Hit")
	bool bHitSuccess{false};

	/* 
	 * Optional hit result.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Hit")
	FHitResult HitResult;
};


USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_SpawnContextEffectsOutput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	TArray<TObjectPtr<UAudioComponent>> AudioComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	TArray<TObjectPtr<UNiagaraComponent>> NiagaraComponents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES")
	TArray<TObjectPtr<UParticleSystemComponent>> ParticlesComponents;
};
