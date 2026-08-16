// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Factories/GGS_AssetFactories.h"

#include "Feedback/GES_ContextEffectsLibrary.h"
#include "Feedback/GES_ContextEffectsPreviewSetting.h"
#include "GenericGameSystemEditor.h"
#include "Interaction/GGS_InteractionDefinition.h"
#include "Settings/GSS_SettingsDefinition.h"
#include "SettingsUI/GSS_SettingsDetailSectionBuilder.h"
#include "SettingsUI/GSS_SettingsEntryWidgetFactory.h"
#include "UI/Actions/GUIS_UIActionFactory.h"
#include "UI/Common/GUIS_DetailSectionsBuilder.h"
#include "Utilities/GGS_SocketRelationshipMapping.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GGS_AssetFactories)

#define LOCTEXT_NAMESPACE "GGS_AssetFactories"

UGGS_AssetFactoryBase::UGGS_AssetFactoryBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UGGS_AssetFactoryBase::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext*)
{
	check(Class && SupportedClass && Class->IsChildOf(SupportedClass));
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional, Context);
}

uint32 UGGS_AssetFactoryBase::GetMenuCategories() const
{
	return FGenericGameSystemEditorModule::GetAssetCategory();
}

const TArray<FText>& UGGS_AssetFactoryBase::GetMenuCategorySubMenus() const
{
	return MenuCategorySubMenus;
}

void UGGS_AssetFactoryBase::SetMenuCategorySubMenu(const FText& SubMenu)
{
	MenuCategorySubMenus = { SubMenu };
}

#define GGS_IMPLEMENT_FACTORY(ClassName, AssetClass, SubMenu, NameKey, DisplayName, ToolTipKey, ToolTip) \
	ClassName::ClassName(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { SupportedClass = AssetClass::StaticClass(); SetMenuCategorySubMenu(LOCTEXT(#SubMenu, #SubMenu)); } \
	FText ClassName::GetDisplayName() const { return LOCTEXT(NameKey, DisplayName); } \
	FText ClassName::GetToolTip() const { return LOCTEXT(ToolTipKey, ToolTip); }

GGS_IMPLEMENT_FACTORY(UGGS_ContextEffectsLibraryFactory, UGES_ContextEffectsLibrary, Effects, "ContextEffectsLibraryName", "Context Effects Library", "ContextEffectsLibraryToolTip", "Creates a library of Gameplay Tag-driven context visual and sound effects.")
GGS_IMPLEMENT_FACTORY(UGGS_ContextEffectsPreviewSettingFactory, UGES_ContextEffectsPreviewSetting, Effects, "ContextEffectsPreviewName", "Context Effects Preview Settings", "ContextEffectsPreviewToolTip", "Creates preview settings for context effects libraries.")
GGS_IMPLEMENT_FACTORY(UGGS_InteractionDefinitionFactory, UGGS_InteractionDefinition, Gameplay, "InteractionDefinitionName", "Interaction Definition", "InteractionDefinitionToolTip", "Creates a definition for a gameplay interaction entry.")
GGS_IMPLEMENT_FACTORY(UGGS_SocketRelationshipMappingFactory, UGGS_SocketRelationshipMapping, Gameplay, "SocketRelationshipMappingName", "Socket Relationship Mapping", "SocketRelationshipMappingToolTip", "Creates mesh socket relationship and adjustment mappings.")
GGS_IMPLEMENT_FACTORY(UGGS_UIActionFactoryAssetFactory, UGUIS_UIActionFactory, UI, "UIActionFactoryName", "UI Action Factory", "UIActionFactoryToolTip", "Creates a factory that selects UI actions for data objects.")
GGS_IMPLEMENT_FACTORY(UGGS_DetailSectionBuilderFactory, UGUIS_DetailSectionBuilder_Class, UI, "DetailSectionBuilderName", "Detail Section Builder", "DetailSectionBuilderToolTip", "Creates class-based mappings from data objects to detail sections.")
GGS_IMPLEMENT_FACTORY(UGGS_SettingsDefinitionFactory, UGSS_SettingsDefinitionAsset, Settings, "SettingsDefinitionName", "Settings Definition", "SettingsDefinitionToolTip", "Creates a root Data Asset definition for GSS runtime settings.")
GGS_IMPLEMENT_FACTORY(UGGS_SettingsEntryWidgetFactoryAssetFactory, UGSS_SettingsEntryWidgetFactory, Settings, "SettingsEntryWidgetFactoryName", "Settings List Entry Factory", "SettingsEntryWidgetFactoryToolTip", "Creates mappings from GSS setting classes to list-entry widgets.")
GGS_IMPLEMENT_FACTORY(UGGS_SettingsDetailSectionBuilderFactory, UGSS_SettingsDetailSectionBuilder, Settings, "SettingsDetailSectionBuilderName", "Settings Detail Section Builder", "SettingsDetailSectionBuilderToolTip", "Creates mappings from GSS setting classes to detail sections.")

#undef GGS_IMPLEMENT_FACTORY
#undef LOCTEXT_NAMESPACE
