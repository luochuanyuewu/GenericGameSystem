// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FGenericEffectsSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
