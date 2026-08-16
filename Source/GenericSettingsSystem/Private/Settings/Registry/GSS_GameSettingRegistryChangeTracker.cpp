// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingRegistryChangeTracker.h"

#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValue.h"

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

FGSS_GameSettingRegistryChangeTracker::FGSS_GameSettingRegistryChangeTracker()
{
}

FGSS_GameSettingRegistryChangeTracker::~FGSS_GameSettingRegistryChangeTracker()
{
	if (UGSS_GameSettingRegistry* StrongRegistry = Registry.Get())
	{
		StrongRegistry->OnSettingChangedEvent.RemoveAll(this);
	}
}

void FGSS_GameSettingRegistryChangeTracker::WatchRegistry(UGSS_GameSettingRegistry* InRegistry)
{
	ClearDirtyState();
	StopWatchingRegistry();

	if (Registry.Get() != InRegistry)
	{
		Registry = InRegistry;
		InRegistry->OnSettingChangedEvent.AddRaw(this, &FGSS_GameSettingRegistryChangeTracker::HandleSettingChanged);
	}
}

void FGSS_GameSettingRegistryChangeTracker::StopWatchingRegistry()
{
	if (UGSS_GameSettingRegistry* StrongRegistry = Registry.Get())
	{
		StrongRegistry->OnSettingChangedEvent.RemoveAll(this);
		Registry.Reset();
	}
}

void FGSS_GameSettingRegistryChangeTracker::ClearDirtyState()
{
	ensure(!bRestoringSettings);
	if (bRestoringSettings)
	{
		return;
	}

	bSettingsChanged = false;
	DirtySettings.Reset();
}

void FGSS_GameSettingRegistryChangeTracker::ApplyChanges()
{
	TArray<FObjectKey> AppliedSettings;
	for (auto Entry : DirtySettings)
	{
		if (UGSS_GameSettingValue* SettingValue = Cast<UGSS_GameSettingValue>(Entry.Value))
		{
			if (!SettingValue->Apply())
			{
				continue;
			}
			SettingValue->StoreInitial();
		}
		AppliedSettings.Add(Entry.Key);
	}

	for (const FObjectKey& Key : AppliedSettings)
	{
		DirtySettings.Remove(Key);
	}
	bSettingsChanged = DirtySettings.Num() > 0;
}

void FGSS_GameSettingRegistryChangeTracker::RestoreToInitial()
{
	ensure(!bRestoringSettings);
	if (bRestoringSettings)
	{
		return;
	}

	{
		TGuardValue<bool> LocalGuard(bRestoringSettings, true);
		for (auto Entry : DirtySettings)
		{
			if (UGSS_GameSettingValue* SettingValue = Cast<UGSS_GameSettingValue>(Entry.Value))
			{
				SettingValue->RestoreToInitial();
			}
		}
	}

	ClearDirtyState();
}

void FGSS_GameSettingRegistryChangeTracker::DiscardSetting(UGSS_GameSetting* Setting)
{
	if (Setting)
	{
		DirtySettings.Remove(FObjectKey(Setting));
		bSettingsChanged = DirtySettings.Num() > 0;
	}
}

void FGSS_GameSettingRegistryChangeTracker::HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason)
{
	if (bRestoringSettings)
	{
		return;
	}

	bSettingsChanged = true;
	DirtySettings.Add(FObjectKey(Setting), Setting);
}

#undef LOCTEXT_NAMESPACE
