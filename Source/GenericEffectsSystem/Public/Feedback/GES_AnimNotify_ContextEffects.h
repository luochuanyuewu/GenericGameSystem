// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Chaos/ChaosEngineInterface.h"
#include "GameplayTagContainer.h"
#include "GES_ContextEffectsStructLibrary.h"
#include "Engine/EngineTypes.h"
#include "GES_AnimNotify_ContextEffects.generated.h"

class UGES_AnimNotify_ContextEffects;


/**
 * This class allow you to custom the spawn behavior of context effects.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories, Const)
class UGES_ContextEffectsSpawnParametersProvider : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="GES|AnimNotify")
	bool ProvideParameters(USkeletalMeshComponent* InMeshComp, const UGES_AnimNotify_ContextEffects* InNotifyNotify, UAnimSequenceBase* InAnimation,
	                       FVector& OutSpawnLocation, FRotator& OutSpawnRotation) const;
};

/**
 *
 */
UCLASS(const, hidecategories = Object, CollapseCategories, Config = Game, meta = (DisplayName = "Play Context Effects"))
class GENERICEFFECTSSYSTEM_API UGES_AnimNotify_ContextEffects : public UAnimNotify
{
	GENERATED_BODY()

public:
	UGES_AnimNotify_ContextEffects(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject interface

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;
#endif
	// End UAnimNotify interface

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category="GES|AnimNotify")
	void SetParameters(FGameplayTag EffectIn, FVector LocationOffsetIn, FRotator RotationOffsetIn,
	                   FGES_ContextEffectAnimNotifyVFXSettings VFXPropertiesIn, FGES_ContextEffectAnimNotifyAudioSettings AudioPropertiesIn,
	                   bool bAttachedIn, FName SocketNameIn, bool bPerformTraceIn, FGES_ContextEffectAnimNotifyTraceSettings TracePropertiesIn);
#endif

	// Effect to Play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (DisplayName = "Effect", ExposeOnSpawn = true))
	FGameplayTag Effect;

	//
	/**
	 * Location offset from the socket if attached.
	 * Location offset from mesh location if non-attached.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true))
	FVector LocationOffset = FVector::ZeroVector;

	//Rotation offset from socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true))
	FRotator RotationOffset = FRotator::ZeroRotator;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true))
	FGES_ContextEffectAnimNotifyVFXSettings VFXProperties;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true))
	FGES_ContextEffectAnimNotifyAudioSettings AudioProperties;

	/**
	 * Should attach to the MeshComponent.
	 * If unticked, will spawn based on MeshComponent's location/rotation(or provided by SpawnParameters Provider)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttachmentProperties", meta = (DisplayName="Attach To Mesh", ExposeOnSpawn = true))
	uint32 bAttached : 1; //~ Does not follow coding standard due to redirection from BP

	/**
	 * Optional object to override spawn location/rotation if non-attached.
	 * If no provider, will use mesh component's transform as start location/rotation with offset applied.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="AttachmentProperties", meta=(EditCondition="!bAttached"))
	TObjectPtr<UGES_ContextEffectsSpawnParametersProvider> SpawnParametersProvider;

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttachmentProperties", meta=(EditCondition="bAttached"))
	FName SocketName{NAME_None};

	/**
	 * Will perform a trace, required for SurfaceType to Context Conversion,If you don't want trace for performance saving, make sure you pass SurfaceType.XXX to contexts.
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true))
	uint32 bPerformTrace : 1;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify", meta = (ExposeOnSpawn = true, EditCondition = "bPerformTrace"))
	FGES_ContextEffectAnimNotifyTraceSettings TraceProperties;

#if WITH_EDITORONLY_DATA

	void PerformEditorPreview(AActor* InOwningActor, FGameplayTagContainer& InSourceContext, FGameplayTagContainer& InTargetContext, USkeletalMeshComponent* InMeshComp);
#endif
};
