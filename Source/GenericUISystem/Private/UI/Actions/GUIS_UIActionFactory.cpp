// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/Actions/GUIS_UIActionFactory.h"

#include "Misc/DataValidation.h"


TArray<FGUIS_UIActionDefinition> UGUIS_UIActionFactory::FindUIActionsForData(const UObject* Data) const
{
	TArray<FGUIS_UIActionDefinition> Ret;

	for (const FGUIS_UIActionDefinition& Definition : UIActionDefinitions)
	{
		if (Definition.EntryAction != nullptr)
		{
			if (Definition.EntryAction->IsCompatible(Data))
			{
				Ret.Add(Definition);
			}
		}
	}

	return Ret;
}


#if WITH_EDITOR
EDataValidationResult UGUIS_UIActionFactory::IsDataValid(FDataValidationContext& Context) const
{
	FText ValidationMessage;
	for (int32 i = 0; i < UIActionDefinitions.Num(); i++)
	{
		if (UIActionDefinitions[i].EntryAction == nullptr)
		{
			Context.AddError(FText::FromString(FString::Format(TEXT("Invalid definition on index:{0}"), {i})));
			return EDataValidationResult::Invalid;
		}
	}
	return Super::IsDataValid(Context);
}
#endif
