// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingRegistry.h"

#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingValue.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingRegistry)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingRegistry
//--------------------------------------

UGSS_GameSettingRegistry::UGSS_GameSettingRegistry()
{
}

void UGSS_GameSettingRegistry::Initialize(ULocalPlayer* InLocalPlayer)
{
	OwningLocalPlayer = InLocalPlayer;
}

bool UGSS_GameSettingRegistry::IsFinishedInitializing() const
{
	bool bReady = true;
	for (UGSS_GameSetting* Setting : RegisteredSettings)
	{
		if (!Setting->IsReady())
		{
			bReady = false;
			break;
		}
	}

	return bReady;
}

void UGSS_GameSettingRegistry::GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings)
{
	TArray<UGSS_GameSetting*> RootSettings;
	if (FilterState.GetSettingRootList().Num() > 0)
	{
		RootSettings.Append(FilterState.GetSettingRootList());
	}
	else
	{
		RootSettings.Append(TopLevelSettings);
	}

	for (UGSS_GameSetting* TopLevelSetting : RootSettings)
	{
		// A page at the root is a navigation destination, not a container to expand. This lets providers
		// independently contribute top-level pages without requiring a shared synthetic root collection.
		if (Cast<UGSS_GameSettingCollectionPage>(TopLevelSetting))
		{
			if (FilterState.DoesSettingPassFilter(*TopLevelSetting))
			{
				InOutSettings.Add(TopLevelSetting);
			}
		}
		else if (const UGSS_GameSettingCollection* TopLevelCollection = Cast<UGSS_GameSettingCollection>(TopLevelSetting))
		{
			TopLevelCollection->GetSettingsForFilter(FilterState, InOutSettings);
		}
		else
		{
			if (FilterState.DoesSettingPassFilter(*TopLevelSetting))
			{
				InOutSettings.Add(TopLevelSetting);
			}
		}
	}
}

UGSS_GameSetting* UGSS_GameSettingRegistry::FindSettingByDevName(const FName& SettingDevName)
{
	for (UGSS_GameSetting* Setting : RegisteredSettings)
	{
		if (Setting->GetDevName() == SettingDevName)
		{
			return Setting;
		}
	}

	return nullptr;
}

UGSS_GameSetting* UGSS_GameSettingRegistry::FindSettingById(const FGameplayTag& SettingId) const
{
	const TObjectPtr<UGSS_GameSetting>* Found = RegisteredSettings.FindByPredicate([&SettingId](const UGSS_GameSetting* Setting)
	{
		return IsValid(Setting) && Setting->GetSettingId() == SettingId;
	});
	return Found ? Found->Get() : nullptr;
}

TArray<UGSS_GameSettingCollection*> UGSS_GameSettingRegistry::GetTopLevelSettingCollections() const
{
	TArray<UGSS_GameSettingCollection*> Collections;
	for (UGSS_GameSetting* TopLevelSetting : TopLevelSettings)
	{
		if (UGSS_GameSettingCollection* Collection = Cast<UGSS_GameSettingCollection>(TopLevelSetting);
			Collection && !Collection->IsA<UGSS_GameSettingCollectionPage>())
		{
			Collections.Add(Collection);
		}
	}
	return Collections;
}

void UGSS_GameSettingRegistry::RegisterSetting(UGSS_GameSetting* InSetting, UGSS_GameSettingCollection* Parent)
{
	if (!InSetting)
	{
		return;
	}

	if (Parent)
	{
		Parent->AddSetting(InSetting);
	}
	else
	{
		TopLevelSettings.Add(InSetting);
	}
	InSetting->SetRegistry(this);
	RegisterSettingTree(InSetting);
	if (OwningLocalPlayer)
	{
		InSetting->Initialize(OwningLocalPlayer);
	}
	else
	{
		InSetting->ResolveEditDependencies();
	}
	ResolveRegisteredEditDependencies();
}

void UGSS_GameSettingRegistry::UnregisterSetting(UGSS_GameSetting* InSetting)
{
	if (!InSetting)
	{
		return;
	}

	TopLevelSettings.Remove(InSetting);
	if (UGSS_GameSettingCollection* ParentCollection = Cast<UGSS_GameSettingCollection>(InSetting->GetSettingParent()))
	{
		ParentCollection->RemoveSetting(InSetting);
	}
	for (UGSS_GameSetting* Child : InSetting->GetChildSettings())
	{
		UnregisterSetting(Child);
	}
	InSetting->OnSettingChangedEvent.RemoveAll(this);
	InSetting->OnSettingAppliedEvent.RemoveAll(this);
	InSetting->OnSettingEditConditionChangedEvent.RemoveAll(this);
	if (UGSS_GameSettingAction* ActionSetting = Cast<UGSS_GameSettingAction>(InSetting))
	{
		ActionSetting->OnExecuteNamedActionEvent.RemoveAll(this);
	}
	else if (UGSS_GameSettingCollectionPage* PageSetting = Cast<UGSS_GameSettingCollectionPage>(InSetting))
	{
		PageSetting->OnExecuteNavigationEvent.RemoveAll(this);
	}
	RegisteredSettings.Remove(InSetting);
	ResolveRegisteredEditDependencies();
}

void UGSS_GameSettingRegistry::ResolveRegisteredEditDependencies()
{
	for (UGSS_GameSetting* Setting : RegisteredSettings)
	{
		if (Setting)
		{
			Setting->ResolveEditDependencies();
		}
	}
}

void UGSS_GameSettingRegistry::ReloadSettingsFromAccessors()
{
	for (UGSS_GameSetting* Setting : RegisteredSettings)
	{
		if (UGSS_GameSettingValue* SettingValue = Cast<UGSS_GameSettingValue>(Setting))
		{
			SettingValue->StoreInitial();
			SettingValue->RestoreToInitial();
		}
	}
}

void UGSS_GameSettingRegistry::RegisterSettingTree(UGSS_GameSetting* InSetting)
{
	InSetting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
	InSetting->OnSettingAppliedEvent.AddUObject(this, &ThisClass::HandleSettingApplied);
	InSetting->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleSettingEditConditionsChanged);

	// Not a fan of this, but it makes sense to aggregate action events for simplicity.
	if (UGSS_GameSettingAction* ActionSetting = Cast<UGSS_GameSettingAction>(InSetting))
	{
		ActionSetting->OnExecuteNamedActionEvent.AddUObject(this, &ThisClass::HandleSettingNamedAction);
	}
	// Not a fan of this, but it makes sense to aggregate navigation events for simplicity.
	else if (UGSS_GameSettingCollectionPage* NewPageCollection = Cast<UGSS_GameSettingCollectionPage>(InSetting))
	{
		NewPageCollection->OnExecuteNavigationEvent.AddUObject(this, &ThisClass::HandleSettingNavigation);
	}

#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!RegisteredSettings.Contains(InSetting), TEXT("This setting has already been registered!"));
	ensureAlwaysMsgf(nullptr == RegisteredSettings.FindByPredicate([&InSetting](UGSS_GameSetting* ExistingSetting) { return InSetting->GetDevName() == ExistingSetting->GetDevName(); }), TEXT("A setting with this DevName has already been registered!  DevNames must be unique within a registry."));
#endif

	RegisteredSettings.Add(InSetting);

	for (UGSS_GameSetting* ChildSetting : InSetting->GetChildSettings())
	{
		RegisterSettingTree(ChildSetting);
	}
}

void UGSS_GameSettingRegistry::HandleSettingApplied(UGSS_GameSetting* Setting)
{
	OnSettingAppliedEvent.Broadcast(Setting);
}

void UGSS_GameSettingRegistry::HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason)
{
	OnSettingChangedEvent.Broadcast(Setting, Reason);
}

void UGSS_GameSettingRegistry::HandleSettingEditConditionsChanged(UGSS_GameSetting* Setting)
{
	OnSettingEditConditionChangedEvent.Broadcast(Setting);
}

void UGSS_GameSettingRegistry::HandleSettingNamedAction(UGSS_GameSetting* Setting, FGameplayTag GameSettings_Action_Tag)
{
	OnSettingNamedActionEvent.Broadcast(Setting, GameSettings_Action_Tag);
}

void UGSS_GameSettingRegistry::HandleSettingNavigation(UGSS_GameSetting* Setting)
{
	OnExecuteNavigationEvent.Broadcast(Setting);
}

#undef LOCTEXT_NAMESPACE
