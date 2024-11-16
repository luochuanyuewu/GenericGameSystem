// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.


#include "GUIS_GenericUISystemSettings.h"
#include "UI/GUIS_GameUIData.h"

const UGUIS_GenericUISystemSettings* UGUIS_GenericUISystemSettings::Get()
{
	return GetDefault<UGUIS_GenericUISystemSettings>();
}

const UGUIS_GameUIData* UGUIS_GenericUISystemSettings::GetGameUIData()
{
	const UGUIS_GenericUISystemSettings* Settings = GetDefault<UGUIS_GenericUISystemSettings>();
	if (Settings->GameUIData.IsNull())
		return nullptr;
	return Settings->GameUIData.LoadSynchronous();
}
