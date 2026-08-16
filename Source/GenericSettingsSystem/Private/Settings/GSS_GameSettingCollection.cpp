// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingCollection.h"
#include "Templates/Casts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingCollection)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingCollection
//--------------------------------------

UGSS_GameSettingCollection::UGSS_GameSettingCollection()
{

}

void UGSS_GameSettingCollection::AddSetting(UGSS_GameSetting* Setting)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(Setting->GetSettingParent() == nullptr, TEXT("This setting already has a parent!"));
	ensureAlwaysMsgf(!Settings.Contains(Setting), TEXT("This collection already includes this setting!"));
#endif

	Settings.Add(Setting);
	Setting->SetSettingParent(this);

	if (LocalPlayer)
	{
		Setting->Initialize(LocalPlayer);
	}
}

void UGSS_GameSettingCollection::RemoveSetting(UGSS_GameSetting* Setting)
{
	if (Setting)
	{
		Settings.Remove(Setting);
		Setting->SetSettingParent(nullptr);
	}
}

TArray<UGSS_GameSettingCollection*> UGSS_GameSettingCollection::GetChildCollections() const
{
	TArray<UGSS_GameSettingCollection*> CollectionSettings;

	for (UGSS_GameSetting* ChildSetting : Settings)
	{
		if (UGSS_GameSettingCollection* ChildCollection = Cast<UGSS_GameSettingCollection>(ChildSetting))
		{
			CollectionSettings.Add(ChildCollection);
		}
	}

	return CollectionSettings;
}

void UGSS_GameSettingCollection::GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const
{
	for (UGSS_GameSetting* ChildSetting : Settings)
	{
		// If the child setting is a collection, only add it to the set if it has any visible children.
		if (Cast<UGSS_GameSettingCollectionPage>(ChildSetting))
		{
			if (FilterState.DoesSettingPassFilter(*ChildSetting))
			{
				InOutSettings.Add(ChildSetting);
			}
		}
		else if (UGSS_GameSettingCollection* ChildCollection = Cast<UGSS_GameSettingCollection>(ChildSetting))
		{
			TArray<UGSS_GameSetting*> ChildSettings;
			ChildCollection->GetSettingsForFilter(FilterState, ChildSettings);

			if (ChildSettings.Num() > 0)
			{
				// Don't add the root setting to the returned items, it's the container of N-possible 
				// other settings and containers we're actually displaying right now.
				if (!FilterState.IsSettingInRootList(ChildSetting))
				{
					InOutSettings.Add(ChildSetting);
				}

				InOutSettings.Append(ChildSettings);
			}
		}
		else
		{
			if (FilterState.DoesSettingPassFilter(*ChildSetting))
			{
				InOutSettings.Add(ChildSetting);
			}
		}
	}
}

//--------------------------------------
// UGSS_GameSettingCollectionPage
//--------------------------------------

UGSS_GameSettingCollectionPage::UGSS_GameSettingCollectionPage()
{
}

void UGSS_GameSettingCollectionPage::OnInitialized()
{
	Super::OnInitialized();

#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!NavigationText.IsEmpty(), TEXT("You must provide a NavigationText for this setting."));
	ensureAlwaysMsgf(!DescriptionRichText.IsEmpty(), TEXT("You must provide a description for new page collections."));
#endif
}

void UGSS_GameSettingCollectionPage::GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const
{
	// If we're including nested pages, call the super and dump them all, otherwise, we pretend we have none for the filtering.
	// because our settings are displayed on another page.
	if (FilterState.bIncludeNestedPages || FilterState.IsSettingInRootList(this))
	{
		Super::GetSettingsForFilter(FilterState, InOutSettings);
	}
}

void UGSS_GameSettingCollectionPage::ExecuteNavigation()
{
	OnExecuteNavigationEvent.Broadcast(this);
}

#undef LOCTEXT_NAMESPACE
