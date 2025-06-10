// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GUIS_GameUIContext.generated.h"

/**
 * Pieces of data allow you to subclass to add any information you want and shared by many uis.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)

class GENERICUISYSTEM_API UGUIS_GameUIContext : public UObject
{
	GENERATED_BODY()
};
