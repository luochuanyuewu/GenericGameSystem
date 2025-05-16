// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

FString GetGGSLogContextString(const UObject* ContextObject = nullptr);

GENERICGAMESYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogGGS_Interaction, Log, All)

#define INTERACTION_LOG(Verbosity, Format, ...) \
{ \
UE_LOG(LogGGS_Interaction, Verbosity, Format, ##__VA_ARGS__); \
}

// Context rich log for interaction.
#define INTERACTION_RLOG(Verbosity, Format, ...) \
{ \
UE_LOG(LogGGS_Interaction, Verbosity, TEXT("%s " Format " %S"),*GetGGSLogContextString(this), ##__VA_ARGS__,__FUNCTION__) \
}

#define INTERACTION_VRLOG(LogOwner, Verbosity, Format, ...) \
{ \
UE_VLOG(LogOwner,LogGGS_Interaction, Verbosity, TEXT("%s " Format " %S"),*GetGGSLogContextString(LogOwner), ##__VA_ARGS__,__FUNCTION__) \
}