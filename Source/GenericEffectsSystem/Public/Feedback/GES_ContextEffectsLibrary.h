// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/DataAsset.h"
#include "UObject/WeakObjectPtr.h"

#include "GES_ContextEffectsLibrary.generated.h"

class UNiagaraSystem;
class USoundBase;
struct FFrame;

/**
 *
 */
UENUM()
enum class EGES_ContextEffectsLibraryLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GENERICEFFECTSSYSTEM_API FGES_ContextEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES", meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;
};

/**
 *
 */
UCLASS()
class GENERICEFFECTSSYSTEM_API UGES_ActiveContextEffects : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category="GES")
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere, Category="GES")
	FGameplayTagContainer Context;

	UPROPERTY(VisibleAnywhere, Category="GES")
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere, Category="GES")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FGES_ContextEffectLibraryLoadingComplete, TArray<UGES_ActiveContextEffects *>, ActiveContextEffects);

/**
 *
 */
UCLASS(BlueprintType)
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsLibrary : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GES")
	TArray<FGES_ContextEffects> ContextEffects;

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context, TArray<USoundBase *> &Sounds, TArray<UNiagaraSystem *> &NiagaraSystems);

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void LoadEffects();

	EGES_ContextEffectsLibraryLoadState GetContextEffectsLibraryLoadState();

private:
	void LoadEffectsInternal();

	void OnContextEffectLibraryLoadingComplete(TArray<UGES_ActiveContextEffects *> InActiveContextEffects);

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGES_ActiveContextEffects>> ActiveContextEffects;

	UPROPERTY(Transient)
	EGES_ContextEffectsLibraryLoadState EffectsLoadState = EGES_ContextEffectsLibraryLoadState::Unloaded;
};
