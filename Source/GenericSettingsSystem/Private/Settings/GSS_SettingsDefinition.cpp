// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingsDefinition.h"

#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_SettingsProvider.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsDefinition)

void UGSS_SettingsDefinitionAsset::Build(UGSS_SettingsBuilder* Builder) const
{
	if (!Builder) return;
	for (const UGSS_SettingDefinition* Definition : RootSettings)
	{
		if (Definition) Builder->AddDefinition(Definition, nullptr);
	}
}
