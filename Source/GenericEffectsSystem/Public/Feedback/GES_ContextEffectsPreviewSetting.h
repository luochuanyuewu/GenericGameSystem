// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Chaos/ChaosEngineInterface.h"
#include "UObject/Object.h"
#include "GES_ContextEffectsPreviewSetting.generated.h"

/**
 * DataAsset contains context effects preview data.
 */
UCLASS()
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsPreviewSetting : public UDataAsset
{
	GENERATED_BODY()

public:
	// If true, will attempt to match selected Surface Type to Context Tag via Project Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Preview)
	bool bPreviewPhysicalSurfaceAsContext = true;

	// Surface Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Preview, meta = (EditCondition = "bPreviewPhysicalSurfaceAsContext"))
	TEnumAsByte<EPhysicalSurface> PreviewPhysicalSurface = EPhysicalSurface::SurfaceType_Default;

	// Preview Libraries
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Preview, meta = (AllowedClasses = "/Script/GenericEffectsSystem.GES_ContextEffectsLibrary"))
	TArray<FSoftObjectPath> PreviewContextEffectsLibraries;

	// Preview Source Context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Preview)
	FGameplayTagContainer PreviewSourceContext;

	// Preview Target Context
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Preview)
	FGameplayTagContainer PreviewTargetContext;
};
