// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_SmartObjectFunctionLibrary.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorConfig.h"
#include "SmartObjectDefinition.h"
#include "Engine/World.h"
#include "SmartObjectSubsystem.h"

UGameplayBehaviorConfig* UGGS_SmartObjectFunctionLibrary::GetGameplayBehaviorConfig(const USmartObjectBehaviorDefinition* BehaviorDefinition)
{
	if (const UGameplayBehaviorSmartObjectBehaviorDefinition* Definition = Cast<UGameplayBehaviorSmartObjectBehaviorDefinition>(BehaviorDefinition))
	{
		return Definition->GameplayBehaviorConfig;
	}

	return nullptr;
}

bool UGGS_SmartObjectFunctionLibrary::FindGameplayBehaviorConfig(const USmartObjectBehaviorDefinition* BehaviorDefinition, TSubclassOf<UGameplayBehaviorConfig> DesiredClass,
                                                                         UGameplayBehaviorConfig*& OutConfig)
{
	if (UClass* RealClass = DesiredClass)
	{
		if (UGameplayBehaviorConfig* Config = GetGameplayBehaviorConfig(BehaviorDefinition))
		{
			if (Config->GetClass()->IsChildOf(RealClass))
			{
				OutConfig = Config;
				return true;
			}
		}
	}
	return false;
}

bool UGGS_SmartObjectFunctionLibrary::FindInteractionDefinitionFromSlot(UObject* WorldContext, FSmartObjectSlotHandle SmartObjectSlotHandle, FGGS_SmartObjectInteractionEntryData& OutDefinition)
{
	if (WorldContext && WorldContext->GetWorld() && SmartObjectSlotHandle.IsValid())
	{
		if (USmartObjectSubsystem* Subsystem = WorldContext->GetWorld()->GetSubsystem<USmartObjectSubsystem>())
		{
			const FSmartObjectSlotView SlotView = Subsystem->GetSlotView(SmartObjectSlotHandle);
			if (SlotView.IsValid() && SlotView.GetSlotHandle().IsValid())
			{
				if (const FGGS_SmartObjectInteractionEntryData* DefinitionData = SlotView.GetDefinitionDataPtr<FGGS_SmartObjectInteractionEntryData>())
				{
					OutDefinition = *DefinitionData;
					return true;
				}
			}
		}
	}
	return false;
}

