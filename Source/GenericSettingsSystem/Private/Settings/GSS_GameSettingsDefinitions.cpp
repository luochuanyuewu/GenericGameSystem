// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsDefinitions.h"

#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_GameSettingsProvider.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsDefinitions)

void UGSS_GameSettingsDefinitionAsset::Build(UGSS_GameSettingsBuilder* Builder) const
{
	if (!Builder) return;
	for (const UGSS_GameSettingDefinition* Definition : RootSettings)
	{
		if (Definition) Builder->AddDefinition(Definition, nullptr);
	}
}
