// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Feedback/GES_AnimNotify_ContextEffects.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Feedback/GES_ContextEffectsInterface.h"
#include "Feedback/GES_ContextEffectsLibrary.h"
#include "Feedback/GES_ContextEffectsSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GES_AnimNotify_ContextEffects)


UGES_AnimNotify_ContextEffects::UGES_AnimNotify_ContextEffects(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif
}

void UGES_AnimNotify_ContextEffects::PostLoad()
{
	Super::PostLoad();
}

#if WITH_EDITOR
void UGES_AnimNotify_ContextEffects::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FString UGES_AnimNotify_ContextEffects::GetNotifyName_Implementation() const
{
	// If the Effect Tag is valid, pass the string name to the notify name
	if (Effect.IsValid())
	{
		return Effect.ToString();
	}

	return Super::GetNotifyName_Implementation();
}

void UGES_AnimNotify_ContextEffects::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	// Make sure both MeshComp and Owning Actor is valid
	if (AActor* OwningActor = MeshComp->GetOwner())
	{
		// Prepare Trace Data
		bool bHitSuccess = false;
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;

		if (TraceProperties.bIgnoreActor)
		{
			QueryParams.AddIgnoredActor(OwningActor);
		}

		QueryParams.bReturnPhysicalMaterial = true;

		if (bPerformTrace)
		{
			// If trace is needed, set up Start Location to Attached
			FVector TraceStart = bAttached ? MeshComp->GetSocketLocation(SocketName) : MeshComp->GetComponentLocation();

			// Make sure World is valid
			if (UWorld* World = OwningActor->GetWorld())
			{
				// Call Line Trace, Pass in relevant properties
				bHitSuccess = World->LineTraceSingleByChannel(HitResult, TraceStart, (TraceStart + TraceProperties.EndTraceLocationOffset),
				                                              TraceProperties.TraceChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam);
			}
		}

		// Prepare Contexts in advance
		FGameplayTagContainer Contexts;

		// Set up Array of Objects that implement the Context Effects Interface
		TArray<UObject*> Implementers;

		// Determine if the Owning Actor is one of the Objects that implements the Context Effects Interface
		if (OwningActor->Implements<UGES_ContextEffectsInterface>())
		{
			// If so, add it to the Array
			Implementers.Add(OwningActor);
		}

		// Cycle through Owning Actor's Components and determine if any of them is a Component implementing the Context Effect Interface
		for (const auto Component : OwningActor->GetComponents())
		{
			if (Component)
			{
				// If the Component implements the Context Effects Interface, add it to the list
				if (Component->Implements<UGES_ContextEffectsInterface>())
				{
					Implementers.Add(Component);
				}
			}
		}

		// Cycle through all objects implementing the Context Effect Interface
		for (UObject* Implementer : Implementers)
		{
			if (Implementer)
			{
				// If the object is still valid, Execute the AnimMotionEffect Event on it, passing in relevant data
				IGES_ContextEffectsInterface::Execute_AnimMotionEffect(Implementer,
				                                                       (bAttached ? SocketName : FName("None")),
				                                                       Effect, MeshComp, LocationOffset, RotationOffset,
				                                                       Animation, bHitSuccess, HitResult, Contexts, VFXProperties.Scale,
				                                                       AudioProperties.VolumeMultiplier, AudioProperties.PitchMultiplier);
			}
		}

#if WITH_EDITORONLY_DATA
		PerformEditorPreview(OwningActor, Contexts, MeshComp);
#endif
	}
}

#if WITH_EDITOR
void UGES_AnimNotify_ContextEffects::ValidateAssociatedAssets()
{
	Super::ValidateAssociatedAssets();
}

void UGES_AnimNotify_ContextEffects::SetParameters(FGameplayTag EffectIn, FVector LocationOffsetIn, FRotator RotationOffsetIn,
                                                   FGES_ContextEffectAnimNotifyVFXSettings VFXPropertiesIn, FGES_ContextEffectAnimNotifyAudioSettings AudioPropertiesIn,
                                                   bool bAttachedIn, FName SocketNameIn, bool bPerformTraceIn, FGES_ContextEffectAnimNotifyTraceSettings TracePropertiesIn)
{
	Effect = EffectIn;
	LocationOffset = LocationOffsetIn;
	RotationOffset = RotationOffsetIn;
	VFXProperties.Scale = VFXPropertiesIn.Scale;
	AudioProperties.PitchMultiplier = AudioPropertiesIn.PitchMultiplier;
	AudioProperties.VolumeMultiplier = AudioPropertiesIn.VolumeMultiplier;
	bAttached = bAttachedIn;
	SocketName = SocketNameIn;
	bPerformTrace = bPerformTraceIn;
	TraceProperties.EndTraceLocationOffset = TracePropertiesIn.EndTraceLocationOffset;
	TraceProperties.TraceChannel = TracePropertiesIn.TraceChannel;
	TraceProperties.bIgnoreActor = TracePropertiesIn.bIgnoreActor;
}

void UGES_AnimNotify_ContextEffects::PerformEditorPreview(AActor* OwningActor, FGameplayTagContainer& Contexts, USkeletalMeshComponent* MeshComp)
{
	const UGES_ContextEffectsSettings* ContextEffectsSettings = GetDefault<UGES_ContextEffectsSettings>();
	if (ContextEffectsSettings == nullptr)
	{
		return;
	}

	// This is for Anim Editor previewing, it is a deconstruction of the calls made by the Interface and the Subsystem
	if (!ContextEffectsSettings->bPreviewInEditor)
		return;

	UWorld* World = OwningActor->GetWorld();

	// Get the world, make sure it's an Editor Preview World
	if (!World || World->WorldType != EWorldType::EditorPreview)
		return;

	const auto& PreviewProperties = ContextEffectsSettings->PreviewProperties;

	// Add Preview contexts if necessary
	Contexts.AppendTags(PreviewProperties.PreviewContexts);

	// Convert given Surface Type to Context and Add it to the Contexts for this Preview
	if (PreviewProperties.bPreviewPhysicalSurfaceAsContext)
	{
		TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = PreviewProperties.PreviewPhysicalSurface;

		if (const FGameplayTag* SurfaceContextPtr = ContextEffectsSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
		{
			FGameplayTag SurfaceContext = *SurfaceContextPtr;

			Contexts.AddTag(SurfaceContext);
		}
	}

	for (int i = 0; i < PreviewProperties.PreviewContextEffectsLibraries.Num(); ++i)
	{
		// Libraries are soft referenced, so you will want to try to load them now
		if (UObject* EffectsLibrariesObj = PreviewProperties.PreviewContextEffectsLibraries[i].TryLoad())
		{
			// Check if it is in fact a UGES_ContextEffectLibrary type
			if (UGES_ContextEffectsLibrary* EffectLibrary = Cast<UGES_ContextEffectsLibrary>(EffectsLibrariesObj))
			{
				// Prepare Sounds and Niagara System Arrays
				TArray<USoundBase*> TotalSounds;
				TArray<UNiagaraSystem*> TotalNiagaraSystems;

				// Attempt to load the Effect Library content (will cache in Transient data on the Effect Library Asset)
				EffectLibrary->LoadEffects();

				// If the Effect Library is valid and marked as Loaded, Get Effects from it
				if (EffectLibrary && EffectLibrary->GetContextEffectsLibraryLoadState() == EGES_ContextEffectsLibraryLoadState::Loaded)
				{
					// Prepare local arrays
					TArray<USoundBase*> Sounds;
					TArray<UNiagaraSystem*> NiagaraSystems;

					// Get the Effects
					EffectLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);

					// Append to the accumulating arrays
					TotalSounds.Append(Sounds);
					TotalNiagaraSystems.Append(NiagaraSystems);
				}

				// Cycle through Sounds and call Spawn Sound Attached, passing in relevant data
				for (USoundBase* Sound : TotalSounds)
				{
					UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, (bAttached ? SocketName : FName("None")), LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset,
					                                     false, AudioProperties.VolumeMultiplier, AudioProperties.PitchMultiplier, 0.0f, nullptr, nullptr, true);
				}

				// Cycle through Niagara Systems and call Spawn System Attached, passing in relevant data
				for (UNiagaraSystem* NiagaraSystem : TotalNiagaraSystems)
				{
					UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, MeshComp, (bAttached ? SocketName : FName("None")), LocationOffset,
					                                             RotationOffset, VFXProperties.Scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true, true);
				}
			}
		}
	}
}
#endif
