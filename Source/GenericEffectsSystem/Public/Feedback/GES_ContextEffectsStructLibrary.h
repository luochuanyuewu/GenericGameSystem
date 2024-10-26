// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GES_ContextEffectsEnumLibrary.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	FGameplayTagQuery SourceTagQuery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	FGameplayTagQuery TargetTagQuery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES", meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem, /Script/Engine.ParticleSystem"))
	TArray<FSoftObjectPath> Effects;

	UPROPERTY(VisibleAnywhere, Category="GES", meta=(DisplayName="Context(Deprecated)"))
	FGameplayTagContainer Context;

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

	/**
	 * The source context type determine how passed-in SourceContext will be used during the execution.
	 * if ContextType==Merge, Will merge passed-in SourceContext to implementer's CurrentContexts as final source context for fx query.
	 * if Context==Override, Will only use passed-in SourceContext as source context for fx query.
	 * 源情景类型决定传入的SourceContext会被如何使用。
	 * 如果等于Merge，则是将传入SourceContext与实现者的CurrentContexts合并，并将合并结果作为查询效果的依据。
	 * 如果等于Override，则只会将传入的SourceContext作为查询效果的依据。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Context")
	EGES_EffectsContextType SourceContextType{EGES_EffectsContextType::Merge};

	/**
	 * Optional SourceContext, usage will be depended on SourceContextType.
	 * 可选的源情景，用法会取决于SourceContextType的类型。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GES|Context")
	FGameplayTagContainer SourceContext;

	/**
	 * Optional TargetContext.
	 * 可选的目标情景。
	 */
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
