// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "AssetDefinitions/GGS_AssetDefinitions.h"

#include "Feedback/GES_ContextEffectsLibrary.h"
#include "Feedback/GES_ContextEffectsPreviewSetting.h"
#include "Interaction/GGS_InteractionDefinition.h"
#include "Settings/GSS_SettingsDefinition.h"
#include "SettingsUI/GSS_SettingsDetailSectionBuilder.h"
#include "SettingsUI/GSS_SettingsEntryWidgetFactory.h"
#include "UI/Actions/GUIS_UIActionFactory.h"
#include "UI/Common/GUIS_DetailSectionsBuilder.h"
#include "Utilities/GGS_SocketRelationshipMapping.h"

namespace GGSAssetDefinition
{
	static TConstArrayView<FAssetCategoryPath> EffectsCategories()
	{
		static const auto Categories = { FAssetCategoryPath(NSLOCTEXT("GGSAssetDefinition", "Root", "Generic Game System"), NSLOCTEXT("GGSAssetDefinition", "Effects", "Effects")) };
		return Categories;
	}

	static TConstArrayView<FAssetCategoryPath> GameplayCategories()
	{
		static const auto Categories = { FAssetCategoryPath(NSLOCTEXT("GGSAssetDefinition", "Root", "Generic Game System"), NSLOCTEXT("GGSAssetDefinition", "Gameplay", "Gameplay")) };
		return Categories;
	}

	static TConstArrayView<FAssetCategoryPath> UICategories()
	{
		static const auto Categories = { FAssetCategoryPath(NSLOCTEXT("GGSAssetDefinition", "Root", "Generic Game System"), NSLOCTEXT("GGSAssetDefinition", "UI", "UI")) };
		return Categories;
	}

	static TConstArrayView<FAssetCategoryPath> SettingsCategories()
	{
		static const auto Categories = { FAssetCategoryPath(NSLOCTEXT("GGSAssetDefinition", "Root", "Generic Game System"), NSLOCTEXT("GGSAssetDefinition", "Settings", "Settings")) };
		return Categories;
	}
}

#define GGS_IMPLEMENT_ASSET_DEFINITION(ClassName, AssetClass, DisplayNameKey, DisplayName, CategoryFunction, Color) \
	FText ClassName::GetAssetDisplayName() const { return NSLOCTEXT("GGSAssetDefinition", DisplayNameKey, DisplayName); } \
	FLinearColor ClassName::GetAssetColor() const { return Color; } \
	TSoftClassPtr<UObject> ClassName::GetAssetClass() const { return AssetClass::StaticClass(); } \
	TConstArrayView<FAssetCategoryPath> ClassName::GetAssetCategories() const { return GGSAssetDefinition::CategoryFunction(); }

GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_ContextEffectsLibrary, UGES_ContextEffectsLibrary, "ContextEffectsLibrary", "Context Effects Library", EffectsCategories, FLinearColor(0.30f, 0.75f, 1.00f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_ContextEffectsPreviewSetting, UGES_ContextEffectsPreviewSetting, "ContextEffectsPreviewSetting", "Context Effects Preview Settings", EffectsCategories, FLinearColor(0.30f, 0.75f, 1.00f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_InteractionDefinition, UGGS_InteractionDefinition, "InteractionDefinition", "Interaction Definition", GameplayCategories, FLinearColor(0.70f, 0.45f, 1.00f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_SocketRelationshipMapping, UGGS_SocketRelationshipMapping, "SocketRelationshipMapping", "Socket Relationship Mapping", GameplayCategories, FLinearColor(0.70f, 0.45f, 1.00f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_UIActionFactory, UGUIS_UIActionFactory, "UIActionFactory", "UI Action Factory", UICategories, FLinearColor(0.25f, 0.90f, 0.80f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_DetailSectionBuilder, UGUIS_DetailSectionBuilder_Class, "DetailSectionBuilder", "Detail Section Builder", UICategories, FLinearColor(0.25f, 0.90f, 0.80f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_SettingsDefinition, UGSS_SettingsDefinitionAsset, "SettingsDefinition", "Settings Definition", SettingsCategories, FLinearColor(1.00f, 0.70f, 0.25f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_SettingsEntryWidgetFactory, UGSS_SettingsEntryWidgetFactory, "SettingsEntryWidgetFactory", "Settings List Entry Factory", SettingsCategories, FLinearColor(1.00f, 0.70f, 0.25f))
GGS_IMPLEMENT_ASSET_DEFINITION(UGGS_AssetDefinition_SettingsDetailSectionBuilder, UGSS_SettingsDetailSectionBuilder, "SettingsDetailSectionBuilder", "Settings Detail Section Builder", SettingsCategories, FLinearColor(1.00f, 0.70f, 0.25f))

#undef GGS_IMPLEMENT_ASSET_DEFINITION
