// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GES_AnimNotify_ContextEffects.h"
#include "Subsystems/WorldSubsystem.h"

#include "GES_ContextEffectsSubsystem.generated.h"

enum EPhysicalSurface : int;

class AActor;
class UAudioComponent;
class UGES_ContextEffectsLibrary;
class UNiagaraComponent;
class USceneComponent;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;

/**
 *
 */
UCLASS(config = Game, defaultconfig)
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//
	UPROPERTY(config, EditAnywhere, Category="GES")
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Config, EditAnywhere, Category="PreviewProperties")
	uint32 bPreviewInEditor : 1;

	UPROPERTY(EditAnywhere, Category="PreviewProperties", meta = (EditCondition = "bPreviewInEditor"))
	FGES_ContextEffectAnimNotifyPreviewSettings PreviewProperties;
#endif
};

/**
 *
 */
UCLASS()
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsSet : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	TSet<TObjectPtr<UGES_ContextEffectsLibrary>> ContextEffectsLibraries;
};

/**
 *
 */
UCLASS()
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void SpawnContextEffects(
		const AActor *SpawningActor, USceneComponent *AttachToComponent, const FName AttachPoint, const FVector LocationOffset, const FRotator RotationOffset, FGameplayTag Effect, FGameplayTagContainer Contexts, TArray<UAudioComponent *> &AudioOut, TArray<UNiagaraComponent *> &NiagaraOut, FVector VFXScale = FVector(1), float AudioVolume = 1, float AudioPitch = 1);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag &Context);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void LoadAndAddContextEffectsLibraries(AActor *OwningActor, TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> ContextEffectsLibraries);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void UnloadAndRemoveContextEffectsLibraries(AActor *OwningActor);

private:
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UGES_ContextEffectsSet>> ActiveActorEffectsMap;
};
