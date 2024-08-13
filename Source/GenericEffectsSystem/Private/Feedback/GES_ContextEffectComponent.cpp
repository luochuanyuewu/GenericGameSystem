// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Feedback/GES_ContextEffectComponent.h"

#include "GameplayTagAssetInterface.h"
#include "Engine/World.h"
#include "Feedback/GES_ContextEffectsSubsystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GES_LogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GES_ContextEffectComponent)

class UAnimSequenceBase;
class USceneComponent;


// Sets default values for this component's properties
UGES_ContextEffectComponent::UGES_ContextEffectComponent()
{
	// Disable component tick, enable Auto Activate
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	// ...
}


// Called when the game starts
void UGES_ContextEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentContexts.AppendTags(DefaultEffectContexts);
	CurrentContextEffectsLibraries = DefaultContextEffectsLibraries;

	// On Begin Play, Load and Add Context Effects pairings
	if (const UWorld* World = GetWorld())
	{
		if (UGES_ContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UGES_ContextEffectsSubsystem>())
		{
			ContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
	if (bAutoSetupTagsProvider)
	{
		SetupTagsProvider();
	}
}

void UGES_ContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// On End PLay, remove unnecessary context effects pairings
	if (const UWorld* World = GetWorld())
	{
		if (UGES_ContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UGES_ContextEffectsSubsystem>())
		{
			ContextEffectsSubsystem->UnloadAndRemoveContextEffectsLibraries(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UGES_ContextEffectComponent::SetupTagsProvider()
{
	if (GetOwner()->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		SetGameplayTagsProvider(GetOwner());
	}
	else
	{
		TArray<UActorComponent*> Components = GetOwner()->GetComponentsByInterface(UGameplayTagAssetInterface::StaticClass());
		if (Components.IsValidIndex(0))
		{
			SetGameplayTagsProvider(Components[0]);
		}
	}
}

// Implementation of Interface's AnimMotionEffect function
void UGES_ContextEffectComponent::AnimMotionEffect_Implementation(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent,
                                                                  const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase* AnimationSequence,
                                                                  const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts,
                                                                  FVector VFXScale, float AudioVolume, float AudioPitch)
{
	// Prep Components
	TArray<UAudioComponent*> AudioComponentsToAdd;
	TArray<UNiagaraComponent*> NiagaraComponentsToAdd;

	FGameplayTagContainer TotalContexts;

	// Aggregate contexts
	TotalContexts.AppendTags(Contexts);
	TotalContexts.AppendTags(CurrentContexts);
	if (IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(GameplayTagsProvider))
	{
		FGameplayTagContainer RetTags;
		TagAssetInterface->GetOwnedGameplayTags(RetTags);
		TotalContexts.AppendTags(RetTags);
	}

	// Check if converting Physical Surface Type to Context
	if (bConvertPhysicalSurfaceToContext)
	{
		// Get Phys Mat Type Pointer
		TWeakObjectPtr<UPhysicalMaterial> PhysicalSurfaceTypePtr = HitResult.PhysMaterial;

		// Check if pointer is okay
		if (PhysicalSurfaceTypePtr.IsValid())
		{
			// Get the Surface Type Pointer
			TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = PhysicalSurfaceTypePtr->SurfaceType;

			// If Settings are valid
			if (const UGES_ContextEffectsSettings* ContextEffectsSettings = GetDefault<UGES_ContextEffectsSettings>())
			{
				if (ContextEffectsSettings->SurfaceTypeToContextMap.IsEmpty())
				{
					UE_LOG(LogGES, Warning, TEXT("No surface type to context map, Please check ContextEffectsSetting in ProjectSettings!"));
					if (FallbackPhysicalSurface.IsValid())
					{
						TotalContexts.AddTag(FallbackPhysicalSurface);
					}
				}
				else
				{
					// Convert Surface Type to known
					if (const FGameplayTag* SurfaceContextPtr = ContextEffectsSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
					{
						FGameplayTag SurfaceContext = *SurfaceContextPtr;

						TotalContexts.AddTag(SurfaceContext);
					}
					else
					{
						UE_LOG(LogGES, Warning, TEXT("No surface type(%d) to context map found, Please check ContextEffectsSetting in ProjectSettings!"), PhysicalSurfaceType.GetValue());
						if (FallbackPhysicalSurface.IsValid())
						{
							TotalContexts.AddTag(FallbackPhysicalSurface);
						}
					}
				}
			}
		}
		else
		{
			if (FallbackPhysicalSurface.IsValid())
			{
				TotalContexts.AddTag(FallbackPhysicalSurface);
			}
		}
	}

	// Cycle through Active Audio Components and cache
	for (UAudioComponent* ActiveAudioComponent : ActiveAudioComponents)
	{
		if (ActiveAudioComponent)
		{
			AudioComponentsToAdd.Add(ActiveAudioComponent);
		}
	}

	// Cycle through Active Niagara Components and cache
	for (UNiagaraComponent* ActiveNiagaraComponent : ActiveNiagaraComponents)
	{
		if (ActiveNiagaraComponent)
		{
			NiagaraComponentsToAdd.Add(ActiveNiagaraComponent);
		}
	}

	// Get World
	if (const UWorld* World = GetWorld())
	{
		// Get Subsystem
		if (UGES_ContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UGES_ContextEffectsSubsystem>())
		{
			// Set up Audio Components and Niagara
			TArray<UAudioComponent*> AudioComponents;
			TArray<UNiagaraComponent*> NiagaraComponents;

			// Spawn effects
			ContextEffectsSubsystem->SpawnContextEffects(GetOwner(), StaticMeshComponent, Bone,
			                                             LocationOffset, RotationOffset, MotionEffect, TotalContexts,
			                                             AudioComponents, NiagaraComponents, VFXScale, AudioVolume, AudioPitch);

			// Append resultant effects
			AudioComponentsToAdd.Append(AudioComponents);
			NiagaraComponentsToAdd.Append(NiagaraComponents);
		}
	}

	// Append Active Audio Components
	ActiveAudioComponents.Empty();
	ActiveAudioComponents.Append(AudioComponentsToAdd);

	// Append Active
	ActiveNiagaraComponents.Empty();
	ActiveNiagaraComponents.Append(NiagaraComponentsToAdd);
}

void UGES_ContextEffectComponent::SetGameplayTagsProvider(UObject* Provider)
{
	if (!IsValid(Provider))
	{
		UE_LOG(LogGES, Warning, TEXT("Passed invalid GameplayTagsProvider. Actor:%s  %S"), *GetName(), __FUNCTION__);
		return;
	}
	if (IGameplayTagAssetInterface* TagAssetInterface = Cast<IGameplayTagAssetInterface>(Provider))
	{
		GameplayTagsProvider = Provider;
	}
	else
	{
		UE_LOG(LogGES, Warning, TEXT("Passed in GameplayTagsProvider(%s) Doesn't implement GameplayTagAssetInterface, it can't provide gameplay tags. Actor:%s  %S"), *Provider->GetClass()->GetName(),
		       *GetName(), __FUNCTION__);
		return;
	}
}

void UGES_ContextEffectComponent::UpdateEffectContexts(FGameplayTagContainer NewEffectContexts)
{
	// Reset and update
	CurrentContexts.Reset(NewEffectContexts.Num());
	CurrentContexts.AppendTags(NewEffectContexts);
}

void UGES_ContextEffectComponent::UpdateLibraries(
	TSet<TSoftObjectPtr<UGES_ContextEffectsLibrary>> NewContextEffectsLibraries)
{
	// Clear out existing Effects
	CurrentContextEffectsLibraries = NewContextEffectsLibraries;

	// Get World
	if (const UWorld* World = GetWorld())
	{
		// Get Subsystem
		if (UGES_ContextEffectsSubsystem* ContextEffectsSubsystem = World->GetSubsystem<UGES_ContextEffectsSubsystem>())
		{
			// Load and Add Libraries to Subsystem                  
			ContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
}
