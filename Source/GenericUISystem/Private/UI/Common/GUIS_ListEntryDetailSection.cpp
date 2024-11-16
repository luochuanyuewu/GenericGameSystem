// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/Common/GUIS_ListEntryDetailSection.h"


void UGUIS_ListEntryDetailSection::SetListItemObject(UObject* ListItemObject)
{
	NativeOnListItemObjectSet(ListItemObject);
}

void UGUIS_ListEntryDetailSection::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	OnListItemObjectSet(ListItemObject);
}

