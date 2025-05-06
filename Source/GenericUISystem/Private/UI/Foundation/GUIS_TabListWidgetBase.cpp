// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#include "UI/Foundation/GUIS_TabListWidgetBase.h"

#include "CommonAnimatedSwitcher.h"
#include "CommonButtonBase.h"
#include "CommonActivatableWidget.h"
#include "GUIS_LogChannels.h"
#include "Editor/WidgetCompilerLog.h"
#include "UI/Foundation/GUIS_TabDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_TabListWidgetBase)

void UGUIS_TabListWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGUIS_TabListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetupTabs();
}

void UGUIS_TabListWidgetBase::NativeDestruct()
{
	for (UGUIS_TabDefinition* TabInfo : TabDefinitions)
	{
		if (TabInfo->CreatedTabContentWidget)
		{
			TabInfo->CreatedTabContentWidget->RemoveFromParent();
			TabInfo->CreatedTabContentWidget = nullptr;
		}
	}

	Super::NativeDestruct();
}

// bool UGUIS_TabListWidgetBase::GetPreregisteredTabInfo(const FName TabNameId, FGUIS_TabDescriptor& OutTabInfo)
// {
// 	const FGUIS_TabDescriptor* const FoundTabInfo = PreregisteredTabInfoArray.FindByPredicate([&](FGUIS_TabDescriptor& TabInfo) -> bool
// 	{
// 		return TabInfo.TabId == TabNameId;
// 	});
//
// 	if (!FoundTabInfo)
// 	{
// 		return false;
// 	}
//
// 	OutTabInfo = *FoundTabInfo;
// 	return true;
// }

bool UGUIS_TabListWidgetBase::GetTabDefinition(const FName TabNameId, UGUIS_TabDefinition*& OutTabDefinition)
{
	const TObjectPtr<UGUIS_TabDefinition>* FoundTabInfo = TabDefinitions.FindByPredicate([&](const TObjectPtr<UGUIS_TabDefinition> TabInfo)-> bool
	{
		return TabInfo && TabInfo->TabId == TabNameId;
	});

	if (!FoundTabInfo)
	{
		return false;
	}

	OutTabDefinition = *FoundTabInfo;
	return true;
}

void UGUIS_TabListWidgetBase::SetTabHiddenState(FName TabNameId, bool bHidden)
{
	for (UGUIS_TabDefinition* TabInfo : TabDefinitions)
	{
		if (TabInfo->TabId == TabNameId)
		{
			TabInfo->bHidden = bHidden;
			break;
		}
	}
}

bool UGUIS_TabListWidgetBase::RegisterDynamicTab(UGUIS_TabDefinition* TabDefinition)
{
	if (!IsValid(TabDefinition))
	{
		return false;
	}
	// If it's hidden we just ignore it.
	if (TabDefinition->bHidden)
	{
		return true;
	}

	PendingTabLabelInfoMap.Add(TabDefinition->TabId, TabDefinition);

	return RegisterTab(TabDefinition->TabId, TabDefinition->TabButtonType.LoadSynchronous(), TabDefinition->CreatedTabContentWidget);
}

void UGUIS_TabListWidgetBase::HandlePreLinkedSwitcherChanged()
{
	for (UGUIS_TabDefinition* TabInfo : TabDefinitions)
	{
		// Remove tab content widget from linked switcher, as it is being disassociated.
		if (TabInfo->CreatedTabContentWidget)
		{
			TabInfo->CreatedTabContentWidget->RemoveFromParent();
		}
	}

	Super::HandlePreLinkedSwitcherChanged();
}

void UGUIS_TabListWidgetBase::HandlePostLinkedSwitcherChanged()
{
	if (!IsDesignTime() && GetCachedWidget().IsValid())
	{
		// Don't bother making tabs if we're in the designer or haven't been constructed yet
		SetupTabs();
	}

	Super::HandlePostLinkedSwitcherChanged();
}

void UGUIS_TabListWidgetBase::HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton)
{
	UGUIS_TabDefinition* TabDefinition = nullptr;

	if (!GetTabDefinition(TabId, TabDefinition))
	{
		TabDefinition = *PendingTabLabelInfoMap.Find(TabId);
	}

	if (TabButton->GetClass()->ImplementsInterface(UGUIS_TabButtonInterface::StaticClass()))
	{
		if (ensureMsgf(TabDefinition, TEXT("A tab button was created with id %s but no label info was specified. RegisterDynamicTab should be used over RegisterTab to provide label info."),
		               *TabId.ToString()))
		{
			IGUIS_TabButtonInterface::Execute_SetTabDefinition(TabButton, TabDefinition);
		}
	}

	PendingTabLabelInfoMap.Remove(TabId);
}

bool UGUIS_TabListWidgetBase::IsFirstTabActive() const
{
	if (TabDefinitions.Num() > 0)
	{
		return GetActiveTab() == TabDefinitions[0]->TabId;
	}

	return false;
}

bool UGUIS_TabListWidgetBase::IsLastTabActive() const
{
	if (TabDefinitions.Num() > 0)
	{
		return GetActiveTab() == TabDefinitions.Last()->TabId;
	}

	return false;
}

bool UGUIS_TabListWidgetBase::IsTabVisible(FName TabId)
{
	if (const UCommonButtonBase* Button = GetTabButtonBaseByID(TabId))
	{
		const ESlateVisibility TabVisibility = Button->GetVisibility();
		return (TabVisibility == ESlateVisibility::Visible
			|| TabVisibility == ESlateVisibility::HitTestInvisible
			|| TabVisibility == ESlateVisibility::SelfHitTestInvisible);
	}

	return false;
}

int32 UGUIS_TabListWidgetBase::GetVisibleTabCount()
{
	int32 Result = 0;
	const int32 TabCount = GetTabCount();
	for (int32 Index = 0; Index < TabCount; Index++)
	{
		if (IsTabVisible(GetTabIdAtIndex(Index)))
		{
			Result++;
		}
	}

	return Result;
}

void UGUIS_TabListWidgetBase::SetupTabs()
{
	for (UGUIS_TabDefinition* TabInfo : TabDefinitions)
	{
		if (TabInfo->bHidden)
		{
			continue;
		}

		// If the tab content hasn't been created already, create it.
		if (!TabInfo->CreatedTabContentWidget && !TabInfo->TabContentType.IsNull())
		{
			TabInfo->CreatedTabContentWidget = CreateWidget<UCommonUserWidget>(GetOwningPlayer(), TabInfo->TabContentType.LoadSynchronous());
			OnTabContentCreatedNative.Broadcast(TabInfo->TabId, Cast<UCommonUserWidget>(TabInfo->CreatedTabContentWidget));
			OnTabContentCreated.Broadcast(TabInfo->TabId, Cast<UCommonUserWidget>(TabInfo->CreatedTabContentWidget));
		}

		if (UCommonAnimatedSwitcher* CurrentLinkedSwitcher = GetLinkedSwitcher())
		{
			// Add the tab content to the newly linked switcher.
			if (!CurrentLinkedSwitcher->HasChild(TabInfo->CreatedTabContentWidget))
			{
				CurrentLinkedSwitcher->AddChild(TabInfo->CreatedTabContentWidget);
			}
		}

		// If the tab is not already registered, register it.
		if (GetTabButtonBaseByID(TabInfo->TabId) == nullptr)
		{
			RegisterTab(TabInfo->TabId, TabInfo->TabButtonType.LoadSynchronous(), TabInfo->CreatedTabContentWidget);
		}
	}
}

#if WITH_EDITOR
void UGUIS_TabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	bool bFoundInvalid = false;
	for (UGUIS_TabDefinition* TabInfo : TabDefinitions)
	{
		if (TabInfo == nullptr)
		{
			bFoundInvalid = true;
			break;
		}
	}
	if (bFoundInvalid)
	{
		CompileLog.Error(FText::FromString(FString::Format(TEXT("Found invalid Tab Definitions,{0} has invalid tab definition"), {GetName()})));
	}
}
#endif
