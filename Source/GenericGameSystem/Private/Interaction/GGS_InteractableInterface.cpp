// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/GGS_InteractableInterface.h"


// Add default functionality here for any IGGS_InteractableInterface functions that are not pure virtual.

FText IGGS_InteractableInterface::GetInteractionDisplayNameText_Implementation() const
{
	if (UObject* Object = _getUObject())
	{
		return FText::FromString(GetNameSafe(Object));
	}
	return FText::GetEmpty();
}
