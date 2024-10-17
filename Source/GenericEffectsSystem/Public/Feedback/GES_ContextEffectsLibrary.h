// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "GES_ContextEffectsEnumLibrary.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/DataAsset.h"
#include "UObject/WeakObjectPtr.h"
#include "GES_ContextEffectsStructLibrary.h"
#include "GES_ContextEffectsLibrary.generated.h"

class UNiagaraSystem;
class USoundBase;
struct FFrame;




/**
 * An instance of context effects.
 */
UCLASS(DisplayName="GMS Context Effects Instance")
class GENERICEFFECTSSYSTEM_API UGES_ActiveContextEffects : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category="GES")
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere, Category="GES")
	FGameplayTagContainer SourceContext;

	UPROPERTY(VisibleAnywhere, Category="GES")
	FGameplayTagContainer TargetContext;

	UPROPERTY(VisibleAnywhere, Category="GES")
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere, Category="GES")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;
	
	UPROPERTY(VisibleAnywhere, Category="GES")
	TArray<TObjectPtr<UParticleSystem>> ParticleSystems;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FGES_ContextEffectLibraryLoadingComplete, TArray<UGES_ActiveContextEffects *>, ActiveContextEffects);

/**
 * A DataAsset which contains many context effects definitions.
 */
UCLASS(BlueprintType)
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsLibrary : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GES", meta = (TitleProperty="EditorFriendlyName"))
	TArray<FGES_ContextEffects> ContextEffects;

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void GetEffects(const FGameplayTag Effect, const FGameplayTagContainer& SourceContext, const FGameplayTagContainer& TargetContext, TArray<USoundBase*>& Sounds,
	                TArray<UNiagaraSystem*>& NiagaraSystems,TArray<UParticleSystem*>& ParticleSystems);

	UFUNCTION(BlueprintCallable, Category="GES|ContextEffect")
	void LoadEffects();

	EGES_ContextEffectsLibraryLoadState GetContextEffectsLibraryLoadState();

private:
	void LoadEffectsInternal();

	void OnContextEffectLibraryLoadingComplete(TArray<UGES_ActiveContextEffects*> InActiveContextEffects);

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGES_ActiveContextEffects>> ActiveContextEffects;

	UPROPERTY(Transient)
	EGES_ContextEffectsLibraryLoadState EffectsLoadState = EGES_ContextEffectsLibraryLoadState::Unloaded;

#if WITH_EDITOR
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};
