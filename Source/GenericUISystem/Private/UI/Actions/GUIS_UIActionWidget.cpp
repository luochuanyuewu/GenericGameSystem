// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "UI/Actions/GUIS_UIActionWidget.h"

#include "Input/CommonUIInputTypes.h"
#include "UI/Actions/GUIS_UIActionFactory.h"

void UGUIS_UIActionWidget::SetAssociatedData(UObject* Data)
{
	AssociatedData = Data;

	// if (IsValid(ActionFactory))
	// {
	// 	
	// }
	//
	// ActionDefinitions = ActionFactory->FindUIActionsForData(AssociatedData.Get());
}

void UGUIS_UIActionWidget::RegisterActions()
{
	if (!IsValid(ActionFactory))
		return;


	if (!AssociatedData.IsValid())
	{
		return;
	}

	TArray<FGUIS_UIActionDefinition> ActionDefinitions = ActionFactory->FindUIActionsForData(AssociatedData.Get());

	for (const FGUIS_UIActionDefinition& Definition : ActionDefinitions)
	{
		if (!Definition.EntryAction)
			continue;
		if (Definition.EntryAction->CanInvoke(AssociatedData.Get(), GetOwningPlayer()))
		{
			FBindUIActionArgs BindArgs(Definition.EntryAction->GetInputActionData(), Definition.bShouldDisplayInActionBar,
			                           FSimpleDelegate::CreateLambda([this,Definition]()
			                           {
				                           HandleUIAction(Definition);
			                           }));

			ActionBindings.Add(RegisterUIActionBinding(BindArgs));
		}
	}
}

void UGUIS_UIActionWidget::UnregisterActions()
{
	for (FUIActionBindingHandle& ActionBinding : ActionBindings)
	{
		ActionBinding.Unregister();
	}

	ActionBindings.Empty();
}

#if WITH_EDITOR
const FText UGUIS_UIActionWidget::GetPaletteCategory()
{
	return FText::FromString(TEXT("Generic UI"));
}
#endif

void UGUIS_UIActionWidget::HandleUIAction(const FGUIS_UIActionDefinition& Definition)
{
	if (AssociatedData.IsValid())
	{
		if (Definition.EntryAction->CanInvoke(AssociatedData.Get(), GetOwningPlayer()))
		{
			Definition.EntryAction->InvokeAction(AssociatedData.Get(), GetOwningPlayer());
		}
	}
}
