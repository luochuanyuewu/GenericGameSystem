// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/Targeting/GGS_TargetingFilterTask_InteractionSmartObjects.h"

#include "SmartObjectBlueprintFunctionLibrary.h"
#include "Interaction/GGS_InteractionSystemComponent.h"

bool UGGS_TargetingFilterTask_InteractionSmartObjects::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (AActor* Actor = TargetData.HitResult.GetActor())
		{
			if (UGGS_InteractionSystemComponent* InteractionSys = UGGS_InteractionSystemComponent::GetInteractionSystemComponent(SourceContext->SourceActor))
			{
				TArray<FSmartObjectRequestResult> Results;
				return !InteractionSys->FindSmartObjectsInActor(Actor,Results);
			}
		}
	}
	return true;
}
