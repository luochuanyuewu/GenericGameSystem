// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingsSubsystem.h"

#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingRegistryChangeTracker.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_SettingsDefinition.h"
#include "Settings/GSS_SettingsDeveloperSettings.h"
#include "Settings/GSS_SettingsProvider.h"
#include "Settings/GSS_SettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsSubsystem)

void UGSS_SettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Registry = NewObject<UGSS_GameSettingRegistry>(this);
	LoadSharedSettings();
	Registry->Initialize(GetLocalPlayer());
	Registry->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChanged);
	Registry->OnSettingAppliedEvent.AddUObject(this, &ThisClass::HandleSettingApplied);
	Registry->OnSettingNamedActionEvent.AddUObject(this, &ThisClass::HandleSettingAction);
	Registry->OnExecuteNavigationEvent.AddUObject(this, &ThisClass::HandleSettingNavigation);
	Registry->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleSettingEditState);
	ChangeTracker = MakeUnique<FGSS_GameSettingRegistryChangeTracker>();
	ChangeTracker->WatchRegistry(Registry);
	if (UGSS_SettingsDefinitionAsset* Root = GetDefault<UGSS_SettingsDeveloperSettings>()->RootDefinition.LoadSynchronous())
	{
		BuildDefinitions(Root);
	}
	for (const TSoftClassPtr<UGSS_SettingsProvider>& ProviderClass : GetDefault<UGSS_SettingsDeveloperSettings>()->StartupProviderClasses)
	{
		if (UClass* LoadedClass = ProviderClass.LoadSynchronous())
		{
			if (UGSS_SettingsProvider* Provider = NewObject<UGSS_SettingsProvider>(this, LoadedClass))
			{
				StartupProviders.Add(Provider);
				RegisterSettingsProvider(Provider);
			}
		}
	}
}

void UGSS_SettingsSubsystem::Deinitialize()
{
	ProviderSettings.Reset();
	StartupProviders.Reset();
	ChangeTracker.Reset();
	SharedSettings = nullptr;
	Registry = nullptr;
	Super::Deinitialize();
}

UGSS_SettingsSubsystem* UGSS_SettingsSubsystem::Get(const UObject* WorldContextObject)
{
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(WorldContextObject))
	{
		return LocalPlayer->GetSubsystem<UGSS_SettingsSubsystem>();
	}
	if (const UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
	{
		if (ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController())
		{
			return LocalPlayer->GetSubsystem<UGSS_SettingsSubsystem>();
		}
	}
	return nullptr;
}

bool UGSS_SettingsSubsystem::GetBoolSettingValue(FGameplayTag SettingId, bool DefaultValue) const
{
	const UGSS_GameSettingValueDiscreteDynamic_Bool* Setting = Registry ? Cast<UGSS_GameSettingValueDiscreteDynamic_Bool>(Registry->FindSettingById(SettingId)) : nullptr;
	return Setting ? Setting->GetValueAsString().ToBool() : DefaultValue;
}

bool UGSS_SettingsSubsystem::SetBoolSettingValue(FGameplayTag SettingId, bool Value)
{
	if (UGSS_GameSettingValueDiscreteDynamic_Bool* Setting = Registry ? Cast<UGSS_GameSettingValueDiscreteDynamic_Bool>(Registry->FindSettingById(SettingId)) : nullptr)
	{
		Setting->SetValueFromString(LexToString(Value));
		return true;
	}
	return false;
}

FString UGSS_SettingsSubsystem::GetStringSettingValue(FGameplayTag SettingId, const FString& DefaultValue) const
{
	if (const UGSS_GameSettingValueDiscreteDynamic* Setting = Registry ? Cast<UGSS_GameSettingValueDiscreteDynamic>(Registry->FindSettingById(SettingId)) : nullptr) return Setting->GetValueAsString();
	return DefaultValue;
}
bool UGSS_SettingsSubsystem::SetStringSettingValue(FGameplayTag SettingId, const FString& Value)
{
	if (UGSS_GameSettingValueDiscreteDynamic* Setting = Registry ? Cast<UGSS_GameSettingValueDiscreteDynamic>(Registry->FindSettingById(SettingId)) : nullptr)
	{
		if (!Setting->HasDynamicOption(Value)) return false;
		Setting->SetValueFromString(Value); return true;
	}
	return false;
}
double UGSS_SettingsSubsystem::GetScalarSettingValue(FGameplayTag SettingId, double DefaultValue) const
{
	if (const UGSS_GameSettingValueScalarDynamic* Setting = Registry ? Cast<UGSS_GameSettingValueScalarDynamic>(Registry->FindSettingById(SettingId)) : nullptr) return Setting->GetValue();
	return DefaultValue;
}
bool UGSS_SettingsSubsystem::SetScalarSettingValue(FGameplayTag SettingId, double Value)
{
	if (UGSS_GameSettingValueScalarDynamic* Setting = Registry ? Cast<UGSS_GameSettingValueScalarDynamic>(Registry->FindSettingById(SettingId)) : nullptr) { Setting->SetValue(Value); return true; }
	return false;
}
bool UGSS_SettingsSubsystem::ExecuteAction(FGameplayTag SettingId)
{
	if (UGSS_GameSettingAction* Setting = Registry ? Cast<UGSS_GameSettingAction>(Registry->FindSettingById(SettingId)) : nullptr) { Setting->ExecuteAction(); return true; }
	return false;
}

void UGSS_SettingsSubsystem::ApplyChanges()
{
	if (!ChangeTracker || !ChangeTracker->HaveSettingsBeenChanged())
	{
		return;
	}
	ChangeTracker->ApplyChanges();
	if (UGameUserSettings* LocalSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		LocalSettings->ApplySettings(false);
	}
	if (SharedSettings)
	{
		SharedSettings->ApplySettings();
		SharedSettings->SaveGameToSlotForLocalPlayer();
	}
}

bool UGSS_SettingsSubsystem::HavePendingChanges() const
{
	return ChangeTracker && ChangeTracker->HaveSettingsBeenChanged();
}

void UGSS_SettingsSubsystem::CancelChanges()
{
	if (!ChangeTracker) return;
	ChangeTracker->RestoreToInitial();
}

void UGSS_SettingsSubsystem::Reload()
{
	LoadSharedSettings();
	if (Registry)
	{
		Registry->ReloadSettingsFromAccessors();
	}
	if (ChangeTracker)
	{
		ChangeTracker->ClearDirtyState();
	}
}

void UGSS_SettingsSubsystem::LoadSharedSettings()
{
	const UGSS_SettingsDeveloperSettings* Settings = GetDefault<UGSS_SettingsDeveloperSettings>();
	const TSubclassOf<UGSS_SettingsShared> SharedClass = Settings->SharedSettingsClass ? Settings->SharedSettingsClass : TSubclassOf<UGSS_SettingsShared>(UGSS_SettingsShared::StaticClass());
	SharedSettings = Cast<UGSS_SettingsShared>(ULocalPlayerSaveGame::LoadOrCreateSaveGameForLocalPlayer(SharedClass, GetLocalPlayer(), Settings->SharedSettingsSlotName));
}

void UGSS_SettingsSubsystem::BuildDefinitions(UGSS_SettingsDefinitionAsset* DefinitionAsset)
{
	UGSS_SettingsBuilder* Builder = NewObject<UGSS_SettingsBuilder>(this);
	Builder->Initialize(this, Registry);
	DefinitionAsset->Build(Builder);
}

FGSS_SettingsRegistrationHandle UGSS_SettingsSubsystem::RegisterSettingsProvider(UGSS_SettingsProvider* Provider)
{
	FGSS_SettingsRegistrationHandle Handle;
	if (!Provider || !Registry)
	{
		return Handle;
	}
	UGSS_SettingsBuilder* Builder = NewObject<UGSS_SettingsBuilder>(this);
	Builder->Initialize(this, Registry);
	Provider->RegisterSettings(Builder);
	Handle.Id = FGuid::NewGuid();
	TArray<TObjectPtr<UGSS_GameSetting>> ProviderRoots;
	for (UGSS_GameSetting* Root : Builder->GetCreatedRootSettings())
	{
		ProviderRoots.Add(Root);
	}
	ProviderSettings.Add(Handle.Id, MoveTemp(ProviderRoots));
	return Handle;
}

void UGSS_SettingsSubsystem::UnregisterSettingsProvider(FGSS_SettingsRegistrationHandle Handle)
{
	if (TArray<TObjectPtr<UGSS_GameSetting>>* Settings = ProviderSettings.Find(Handle.Id))
	{
		for (UGSS_GameSetting* Setting : *Settings)
		{
			if (ChangeTracker)
			{
				ChangeTracker->DiscardSetting(Setting);
			}
			Registry->UnregisterSetting(Setting);
		}
		ProviderSettings.Remove(Handle.Id);
	}
}

void UGSS_SettingsSubsystem::HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason) { if (Setting) OnSettingPendingChanged.Broadcast(Setting->GetSettingId(), Setting, Reason); }
void UGSS_SettingsSubsystem::HandleSettingApplied(UGSS_GameSetting* Setting) { if (Setting) OnSettingApplied.Broadcast(Setting->GetSettingId(), Setting); }
void UGSS_SettingsSubsystem::HandleSettingAction(UGSS_GameSetting* Setting, FGameplayTag) { if (Setting) OnSettingActionExecuted.Broadcast(Setting->GetSettingId(), Setting); }
void UGSS_SettingsSubsystem::HandleSettingNavigation(UGSS_GameSetting* Setting) { if (Setting) OnSettingNavigationRequested.Broadcast(Setting->GetSettingId(), Setting); }
void UGSS_SettingsSubsystem::HandleSettingEditState(UGSS_GameSetting* Setting) { if (Setting) OnSettingEditStateChanged.Broadcast(Setting->GetSettingId(), Setting); }
