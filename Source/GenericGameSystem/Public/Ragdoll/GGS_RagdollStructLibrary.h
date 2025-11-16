// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GGS_RagdollStructLibrary.generated.h"

USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_RagdollState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GGS")
	FVector Velocity{ForceInit};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GGS", Meta = (ForceUnits = "N"))
	float PullForce{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GGS", Meta = (ClampMin = 0))
	int32 SpeedLimitFrameTimeRemaining{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GGS", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float SpeedLimit{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GGS")
	FPoseSnapshot FinalRagdollPose;
};
