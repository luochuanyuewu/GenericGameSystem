// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "GES_ContextEffectsStructLibrary.h"
#include "Subsystems/WorldSubsystem.h"

#include "GES_ContextEffectsSubsystem.generated.h"

class UGES_ContextEffectsPreviewSetting;
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

	UPROPERTY(config, EditAnywhere, Category="PreviewProperties", meta = (EditCondition = "bPreviewInEditor"))
	TSoftObjectPtr<UGES_ContextEffectsPreviewSetting> PreviewSetting;
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
	/**
	 * EffectsLibrary Play ctx vsx/vfx  within single effects library.
	 */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects", meta=(WorldContext = "WorldContextObject"))
	void SpawnContextEffects(UObject* WorldContextObject, TSoftObjectPtr<UGES_ContextEffectsLibrary> EffectsLibrary, FGES_SpawnContextEffectsInput Input, FGES_SpawnContextEffectsOutput& Output);

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void SpawnContextEffectsExt(const AActor* SpawningActor, const FGES_SpawnContextEffectsInput& Input, FGES_SpawnContextEffectsOutput& Output);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> ContextEffectsLibraries);

	/** */
	UFUNCTION(BlueprintCallable, Category="GES|ContextEffects")
	void UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor);

private:
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UGES_ContextEffectsSet>> ActiveActorEffectsMap;
};
