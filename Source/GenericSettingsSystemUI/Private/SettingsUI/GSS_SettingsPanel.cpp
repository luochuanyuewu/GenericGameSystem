// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsPanel.h"

#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_SettingEditCondition.h"
#include "Settings/GSS_GameSettingValue.h"
#include "Settings/GSS_SettingsSubsystem.h"
#include "SettingsUI/GSS_SettingsDetailView.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "UI/Common/GUIS_ListView.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsPanel)

UGSS_SettingsPanel::UGSS_SettingsPanel()
{
	SetIsFocusable(true);
}

void UGSS_SettingsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (List_Settings)
	{
		List_Settings->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::HandleSettingItemHoveredChanged);
		List_Settings->OnItemSelectionChanged().AddUObject(this, &ThisClass::HandleSettingItemSelectionChanged);
	}
}

void UGSS_SettingsPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UnbindRegistryEvents();
	BindRegistryEvents();
}

FReply UGSS_SettingsPanel::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem(); InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad && List_Settings)
	{
		List_Settings->NavigateToIndex(0);
		List_Settings->SetSelectedIndex(0);
		return FReply::Handled();
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UGSS_SettingsPanel::SetSettingsSubsystem(UGSS_SettingsSubsystem* InSubsystem)
{
	if (SettingsSubsystem == InSubsystem)
	{
		return;
	}

	UnbindRegistryEvents();
	if (RefreshHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RefreshHandle);
		RefreshHandle.Reset();
	}
	SettingsSubsystem = InSubsystem;
	Registry = SettingsSubsystem ? SettingsSubsystem->GetRegistry() : nullptr;
	FilterState = FGSS_GameSettingFilterState();
	FilterNavigationStack.Reset();
	LastHoveredOrSelectedSetting = nullptr;
	FillSettingDetails(nullptr);
	BindRegistryEvents();
	RefreshSettingsList();
}

void UGSS_SettingsPanel::ApplyChanges()
{
	if (SettingsSubsystem)
	{
		SettingsSubsystem->ApplyChanges();
	}
}

void UGSS_SettingsPanel::CancelChanges()
{
	if (SettingsSubsystem)
	{
		SettingsSubsystem->CancelChanges();
	}
}

void UGSS_SettingsPanel::ResetVisibleSettingsToDefault()
{
	if (!Registry)
	{
		return;
	}

TArray<UGSS_GameSetting*> Settings;
	Registry->GetSettingsForFilter(FilterState, Settings);
	for (UGSS_GameSetting* Setting : Settings)
	{
		if (UGSS_GameSettingValue* Value = Cast<UGSS_GameSettingValue>(Setting))
		{
			if (Value->GetEditState().IsResettable())
			{
				Value->ResetToDefault();
			}
		}
	}
}

TArray<UGSS_GameSetting*> UGSS_SettingsPanel::GetSettingsWeCanResetToDefault() const
{
	TArray<UGSS_GameSetting*> Settings;
	if (!Registry || !Registry->IsFinishedInitializing())
	{
		return Settings;
	}

	FGSS_GameSettingFilterState AvailableFilter = FilterState;
	AvailableFilter.bIncludeDisabled = true;
	AvailableFilter.bIncludeHidden = true;
	AvailableFilter.bIncludeResetable = false;
	AvailableFilter.bIncludeNestedPages = false;
	Registry->GetSettingsForFilter(AvailableFilter, Settings);
	return Settings;
}

void UGSS_SettingsPanel::SetFilterState(const FGSS_GameSettingFilterState& InFilterState, bool bClearNavigationStack)
{
	FilterState = InFilterState;
	if (bClearNavigationStack)
	{
		FilterNavigationStack.Reset();
	}
	RefreshSettingsList();
}

bool UGSS_SettingsPanel::NavigateToPage(UGSS_GameSettingCollectionPage* Page)
{
	if (!Page || Page == GetCurrentPage())
	{
		return false;
	}

	FilterNavigationStack.Add(FilterState);
	FGSS_GameSettingFilterState PageFilterState;
	PageFilterState.AddSettingToRootList(Page);
	SetFilterState(PageFilterState, false);
	return true;
}

bool UGSS_SettingsPanel::CanPopNavigation() const
{
	return !FilterNavigationStack.IsEmpty();
}

bool UGSS_SettingsPanel::PopNavigation()
{
	if (!CanPopNavigation())
	{
		return false;
	}
	SetFilterState(FilterNavigationStack.Pop(), false);
	return true;
}

UGSS_GameSettingCollectionPage* UGSS_SettingsPanel::GetCurrentPage() const
{
	const TArray<UGSS_GameSetting*>& RootList = FilterState.GetSettingRootList();
	return RootList.Num() == 1 ? Cast<UGSS_GameSettingCollectionPage>(RootList[0]) : nullptr;
}

void UGSS_SettingsPanel::RefreshSettingsList()
{
	if (!List_Settings || RefreshHandle.IsValid())
	{
		return;
	}

	RefreshHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](float)
	{
		if (!Registry || !Registry->IsFinishedInitializing())
		{
			return true;
		}

		TArray<UGSS_GameSetting*> QueriedSettings;
		Registry->GetSettingsForFilter(FilterState, QueriedSettings);
		VisibleSettings.Reset();
		VisibleSettings.Append(QueriedSettings);
		if (!VisibleSettings.Contains(LastHoveredOrSelectedSetting))
		{
			LastHoveredOrSelectedSetting = nullptr;
			FillSettingDetails(nullptr);
		}
		TArray<UObject*> ListItems;
		for (UGSS_GameSetting* Setting : VisibleSettings)
		{
			ListItems.Add(Setting);
		}
		List_Settings->SetListItems(ListItems);

		int32 SelectionIndex = 0;
		if (DesiredSelectionSettingId.IsValid())
		{
			for (int32 Index = 0; Index < VisibleSettings.Num(); ++Index)
			{
				if (VisibleSettings[Index] && VisibleSettings[Index]->GetSettingId() == DesiredSelectionSettingId)
				{
					SelectionIndex = Index;
					break;
				}
			}
			DesiredSelectionSettingId = FGameplayTag();
		}
		if (bAdjustListViewPostRefresh && !VisibleSettings.IsEmpty())
		{
			List_Settings->NavigateToIndex(SelectionIndex);
			List_Settings->SetSelectedIndex(SelectionIndex);
			LastHoveredOrSelectedSetting = VisibleSettings[SelectionIndex];
			FillSettingDetails(LastHoveredOrSelectedSetting);
		}
		bAdjustListViewPostRefresh = true;

		for (UGSS_GameSetting* Setting : VisibleSettings)
		{
			if (Setting)
			{
				Setting->RefreshEditableState(false);
			}
		}

		RefreshHandle.Reset();
		return false;
	}));
}

void UGSS_SettingsPanel::NativeDestruct()
{
	if (RefreshHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RefreshHandle);
		RefreshHandle.Reset();
	}
	UnbindRegistryEvents();
	LastHoveredOrSelectedSetting = nullptr;
	FillSettingDetails(nullptr);
	Super::NativeDestruct();
}

void UGSS_SettingsPanel::HandleNavigationRequested(UGSS_GameSetting* Setting)
{
	if (VisibleSettings.Contains(Setting))
	{
		NavigateToPage(Cast<UGSS_GameSettingCollectionPage>(Setting));
	}
}

void UGSS_SettingsPanel::HandleNamedAction(UGSS_GameSetting* Setting, FGameplayTag ActionId)
{
	OnExecuteNamedAction.Broadcast(Setting, ActionId);
}

void UGSS_SettingsPanel::HandleEditableStateChanged(UGSS_GameSetting* Setting)
{
	const bool bWasVisible = VisibleSettings.Contains(Setting);
	const bool bIsVisible = Setting && Setting->GetEditState().IsVisible();
	if (bWasVisible != bIsVisible)
	{
		bAdjustListViewPostRefresh = Setting ? Setting->GetAdjustListViewPostRefresh() : true;
		RefreshSettingsList();
	}
}

void UGSS_SettingsPanel::HandleSettingItemHoveredChanged(UObject* Item, bool bHovered)
{
	UGSS_GameSetting* Setting = bHovered ? Cast<UGSS_GameSetting>(Item) : LastHoveredOrSelectedSetting.Get();
	if (bHovered && Setting)
	{
		LastHoveredOrSelectedSetting = Setting;
	}
	FillSettingDetails(Setting);
}

void UGSS_SettingsPanel::HandleSettingItemSelectionChanged(UObject* Item)
{
	UGSS_GameSetting* Setting = Cast<UGSS_GameSetting>(Item);
	if (Setting)
	{
		LastHoveredOrSelectedSetting = Setting;
	}
	FillSettingDetails(Setting);
}

void UGSS_SettingsPanel::FillSettingDetails(UGSS_GameSetting* Setting)
{
	if (Details_Settings)
	{
		Details_Settings->FillSettingDetails(Setting);
	}
	OnFocusedSettingChanged.Broadcast(Setting);
}

void UGSS_SettingsPanel::SelectSetting(FGameplayTag SettingId)
{
	DesiredSelectionSettingId = SettingId;
	RefreshSettingsList();
}

UGSS_GameSetting* UGSS_SettingsPanel::GetSelectedSetting() const
{
	return List_Settings ? Cast<UGSS_GameSetting>(List_Settings->GetSelectedItem()) : nullptr;
}

void UGSS_SettingsPanel::BindRegistryEvents()
{
	if (Registry)
	{
		Registry->OnExecuteNavigationEvent.AddUObject(this, &ThisClass::HandleNavigationRequested);
		Registry->OnSettingNamedActionEvent.AddUObject(this, &ThisClass::HandleNamedAction);
		Registry->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleEditableStateChanged);
	}
}

void UGSS_SettingsPanel::UnbindRegistryEvents()
{
	if (Registry)
	{
		Registry->OnExecuteNavigationEvent.RemoveAll(this);
		Registry->OnSettingNamedActionEvent.RemoveAll(this);
		Registry->OnSettingEditConditionChangedEvent.RemoveAll(this);
	}
}
