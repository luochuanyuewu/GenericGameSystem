// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Foundation/GUIS_ButtonBase.h"
#include "GUIS_ListEntry.generated.h"

class UGUIS_UIAction;


/**
 * ListEntry(as CommonButton) will represent a list item(Item of ListView/TileView)
 */
UCLASS(Abstract, meta = (Category = "Generic UI", DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_ListEntry : public UGUIS_ButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()
};
