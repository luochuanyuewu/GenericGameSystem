// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtrTemplates.h"

enum class EGSS_GameSettingChangeReason : uint8;

class UGSS_GameSetting;
class UGSS_GameSettingRegistry;
struct FObjectKey;

/**
 * 
 */
class GENERICSETTINGSSYSTEM_API FGSS_GameSettingRegistryChangeTracker : public FNoncopyable
{
public:
	FGSS_GameSettingRegistryChangeTracker();
	~FGSS_GameSettingRegistryChangeTracker();

	void WatchRegistry(UGSS_GameSettingRegistry* InRegistry);
	void StopWatchingRegistry();

	void ApplyChanges();

	void RestoreToInitial();

	void ClearDirtyState();

	bool IsRestoringSettings() const { return bRestoringSettings; }
	bool HaveSettingsBeenChanged() const { return bSettingsChanged; }

private:
	void HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);

	bool bSettingsChanged = false;
	bool bRestoringSettings = false;

	TWeakObjectPtr<UGSS_GameSettingRegistry> Registry;
	TMap<FObjectKey, TWeakObjectPtr<UGSS_GameSetting>> DirtySettings;
};
