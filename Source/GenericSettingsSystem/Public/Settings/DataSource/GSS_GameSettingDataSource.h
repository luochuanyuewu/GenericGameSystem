// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h"

class ULocalPlayer;

class GENERICSETTINGSSYSTEM_API FGSS_GameSettingDataSource : public TSharedFromThis<FGSS_GameSettingDataSource>
{
public:
	virtual ~FGSS_GameSettingDataSource() { }

	/**
	 * Some settings may take an async amount of time to finish initializing.  The settings system will wait
	 * for all settings to be ready before showing the setting.
	 */
	virtual void Startup(ULocalPlayer* InLocalPlayer, FSimpleDelegate StartupCompleteCallback) { StartupCompleteCallback.ExecuteIfBound(); }

	virtual bool Resolve(ULocalPlayer* InContext) = 0;

	virtual FString GetValueAsString(ULocalPlayer* InContext) const = 0;

	virtual void SetValue(ULocalPlayer* InContext, const FString& Value) = 0;

	virtual FString ToString() const = 0;
};
