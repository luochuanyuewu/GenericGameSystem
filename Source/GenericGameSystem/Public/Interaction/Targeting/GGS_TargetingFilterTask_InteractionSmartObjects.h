// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "GGS_TargetingFilterTask_InteractionSmartObjects.generated.h"

/**
 * A custom task to filter potential interactable actors.
 * The filter comes from the one you specified on interaction system component.
 */
UCLASS(meta=(DisplayName="(GGS)FilterTask:InteractionSmartObject"))
class GENERICGAMESYSTEM_API UGGS_TargetingFilterTask_InteractionSmartObjects : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

protected:
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;
};
