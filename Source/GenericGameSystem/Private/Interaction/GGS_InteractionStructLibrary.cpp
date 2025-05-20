// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_InteractionStructLibrary.h"
#include "Interaction/GGS_InteractionDefinition.h"

FString FGGS_InteractionOption::ToString() const
{
	return FString::Format(TEXT("{0} {1} {2}"),{Definition?Definition->Text.ToString():TEXT("Null Definition"), SlotState==ESmartObjectSlotState::Free?TEXT("Valid"):TEXT("Invalid"),SlotIndex});
}
