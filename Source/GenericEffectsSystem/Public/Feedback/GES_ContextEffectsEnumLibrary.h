// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GES_ContextEffectsEnumLibrary.generated.h"


/**
 *
 */
UENUM()
enum class EGES_ContextEffectsLibraryLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};


UENUM()
enum EGES_EffectsContextMatchType : int
{
	ExactMatch,
	BestMatch
};