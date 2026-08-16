// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "GenericGameSystemEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FGenericGameSystemEditorModule"

uint32 FGenericGameSystemEditorModule::AssetCategory = EAssetTypeCategories::Misc;

void FGenericGameSystemEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GenericGameSystem")), LOCTEXT("AssetCategory", "Generic Game System"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGenericGameSystemEditorModule, GenericGameSystemEditor)
