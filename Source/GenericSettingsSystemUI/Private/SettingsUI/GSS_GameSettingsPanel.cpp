// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingsPanel.h"

#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingEditCondition.h"
#include "SettingsUI/GSS_GameSettingDetailView.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "UI/Common/GUIS_ListView.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsPanel)

UGSS_GameSettingsPanel::UGSS_GameSettingsPanel()
{
	SetIsFocusable(true);
}

void UGSS_GameSettingsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ListView_Settings)
	{
		ListView_Settings->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::HandleSettingItemHoveredChanged);
		ListView_Settings->OnItemSelectionChanged().AddUObject(this, &ThisClass::HandleSettingItemSelectionChanged);
	}
}

void UGSS_GameSettingsPanel::NativeConstruct()
{
	Super::NativeConstruct();
	UnbindRegistryEvents();
	BindRegistryEvents();
}

FReply UGSS_GameSettingsPanel::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem(); InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad && ListView_Settings)
	{
		ListView_Settings->NavigateToIndex(0);
		ListView_Settings->SetSelectedIndex(0);
		return FReply::Handled();
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UGSS_GameSettingsPanel::SetRegistry(UGSS_GameSettingRegistry* InRegistry)
{
	if (Registry == InRegistry)
	{
		return;
	}

	UnbindRegistryEvents();
	if (RefreshHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RefreshHandle);
		RefreshHandle.Reset();
	}
	Registry = InRegistry;
	FilterState = FGSS_GameSettingFilterState();
	FilterNavigationStack.Reset();
	LastHoveredOrSelectedSetting = nullptr;
	FillSettingDetails(nullptr);
	BindRegistryEvents();
	RefreshSettingsList();
}

TArray<UGSS_GameSetting*> UGSS_GameSettingsPanel::GetSettingsWeCanResetToDefault() const
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

void UGSS_GameSettingsPanel::SetFilterState(const FGSS_GameSettingFilterState& InFilterState, bool bClearNavigationStack)
{
	FilterState = InFilterState;
	if (bClearNavigationStack)
	{
		FilterNavigationStack.Reset();
	}
	RefreshSettingsList();
}

bool UGSS_GameSettingsPanel::NavigateToPage(UGSS_GameSettingCollectionPage* Page)
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

bool UGSS_GameSettingsPanel::CanPopNavigationStack() const
{
	return !FilterNavigationStack.IsEmpty();
}

void UGSS_GameSettingsPanel::PopNavigationStack()
{
	if (CanPopNavigationStack())
	{
		SetFilterState(FilterNavigationStack.Pop(), false);
	}
}

UGSS_GameSettingCollectionPage* UGSS_GameSettingsPanel::GetCurrentPage() const
{
	const TArray<UGSS_GameSetting*>& RootList = FilterState.GetSettingRootList();
	return RootList.Num() == 1 ? Cast<UGSS_GameSettingCollectionPage>(RootList[0]) : nullptr;
}

void UGSS_GameSettingsPanel::RefreshSettingsList()
{
	if (!ListView_Settings || RefreshHandle.IsValid())
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
		ListView_Settings->SetListItems(ListItems);

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
			ListView_Settings->NavigateToIndex(SelectionIndex);
			ListView_Settings->SetSelectedIndex(SelectionIndex);
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

void UGSS_GameSettingsPanel::NativeDestruct()
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

void UGSS_GameSettingsPanel::HandleNavigationRequested(UGSS_GameSetting* Setting)
{
	if (VisibleSettings.Contains(Setting))
	{
		NavigateToPage(Cast<UGSS_GameSettingCollectionPage>(Setting));
	}
}

void UGSS_GameSettingsPanel::HandleNamedAction(UGSS_GameSetting* Setting, FGameplayTag ActionId)
{
	BP_OnExecuteNamedAction.Broadcast(Setting, ActionId);
}

void UGSS_GameSettingsPanel::HandleEditableStateChanged(UGSS_GameSetting* Setting)
{
	const bool bWasVisible = VisibleSettings.Contains(Setting);
	const bool bIsVisible = Setting && Setting->GetEditState().IsVisible();
	if (bWasVisible != bIsVisible)
	{
		bAdjustListViewPostRefresh = Setting ? Setting->GetAdjustListViewPostRefresh() : true;
		RefreshSettingsList();
	}
}

void UGSS_GameSettingsPanel::HandleSettingItemHoveredChanged(UObject* Item, bool bHovered)
{
	UGSS_GameSetting* Setting = bHovered ? Cast<UGSS_GameSetting>(Item) : LastHoveredOrSelectedSetting.Get();
	if (bHovered && Setting)
	{
		LastHoveredOrSelectedSetting = Setting;
	}
	FillSettingDetails(Setting);
}

void UGSS_GameSettingsPanel::HandleSettingItemSelectionChanged(UObject* Item)
{
	UGSS_GameSetting* Setting = Cast<UGSS_GameSetting>(Item);
	if (Setting)
	{
		LastHoveredOrSelectedSetting = Setting;
	}
	FillSettingDetails(Setting);
}

void UGSS_GameSettingsPanel::FillSettingDetails(UGSS_GameSetting* Setting)
{
	if (Details_Settings)
	{
		Details_Settings->FillSettingDetails(Setting);
	}
	OnFocusedSettingChanged.Broadcast(Setting);
}

void UGSS_GameSettingsPanel::SelectSetting(FGameplayTag SettingId)
{
	DesiredSelectionSettingId = SettingId;
	RefreshSettingsList();
}

UGSS_GameSetting* UGSS_GameSettingsPanel::GetSelectedSetting() const
{
	return ListView_Settings ? Cast<UGSS_GameSetting>(ListView_Settings->GetSelectedItem()) : nullptr;
}

void UGSS_GameSettingsPanel::BindRegistryEvents()
{
	if (Registry)
	{
		Registry->OnExecuteNavigationEvent.AddUObject(this, &ThisClass::HandleNavigationRequested);
		Registry->OnSettingNamedActionEvent.AddUObject(this, &ThisClass::HandleNamedAction);
		Registry->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleEditableStateChanged);
	}
}

void UGSS_GameSettingsPanel::UnbindRegistryEvents()
{
	if (Registry)
	{
		Registry->OnExecuteNavigationEvent.RemoveAll(this);
		Registry->OnSettingNamedActionEvent.RemoveAll(this);
		Registry->OnSettingEditConditionChangedEvent.RemoveAll(this);
	}
}
