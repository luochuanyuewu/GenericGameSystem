// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/GUIS_GameUIData.h"

TSoftClassPtr<UGUIS_GameModalWidget> UGUIS_GameUIData::FindWidgetClassForModal(FGameplayTag ModelTag) const
{
	if (TaggedModalWidgetClass.Contains(ModelTag))
	{
		return TaggedModalWidgetClass[ModelTag];
	}
	return nullptr;
}
