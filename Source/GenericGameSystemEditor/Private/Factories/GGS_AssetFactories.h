// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Factories/Factory.h"

#include "GGS_AssetFactories.generated.h"

/** Shared right-click factory behavior for GGS Data Assets. / GGS Data Asset 共用的右键 Factory 行为。 */
UCLASS(Abstract)
class UGGS_AssetFactoryBase : public UFactory
{
	GENERATED_BODY()

public:
	UGGS_AssetFactoryBase(const FObjectInitializer& ObjectInitializer);
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual uint32 GetMenuCategories() const override;
	virtual const TArray<FText>& GetMenuCategorySubMenus() const override;
	virtual bool ShouldShowInNewMenu() const override { return true; }

protected:
	void SetMenuCategorySubMenu(const FText& SubMenu);

private:
	TArray<FText> MenuCategorySubMenus;
};

UCLASS()
class UGGS_ContextEffectsLibraryFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_ContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_ContextEffectsPreviewSettingFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_ContextEffectsPreviewSettingFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_InteractionDefinitionFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_InteractionDefinitionFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_SocketRelationshipMappingFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_SocketRelationshipMappingFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_UIActionFactoryAssetFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_UIActionFactoryAssetFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_DetailSectionBuilderFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_DetailSectionBuilderFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_SettingsDefinitionFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_SettingsDefinitionFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_SettingsEntryWidgetFactoryAssetFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_SettingsEntryWidgetFactoryAssetFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
UCLASS()
class UGGS_SettingsDetailSectionBuilderFactory : public UGGS_AssetFactoryBase { GENERATED_BODY() public: UGGS_SettingsDetailSectionBuilderFactory(const FObjectInitializer& ObjectInitializer); virtual FText GetDisplayName() const override; virtual FText GetToolTip() const override; };
