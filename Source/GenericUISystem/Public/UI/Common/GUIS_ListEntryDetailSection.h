// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GUIS_ListEntryDetailSection.generated.h"

/**
 * Many detail sections compose a detail view.
 */
UCLASS(Abstract, meta = (Category = "Generic UI System"))
class GENERICUISYSTEM_API UGUIS_ListEntryDetailSection : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetListItemObject(UObject* ListItemObject);

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject);
	UFUNCTION(BlueprintImplementableEvent)
	void OnListItemObjectSet(UObject* ListItemObject);
};
