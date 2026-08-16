// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "AssetDefinitionDefault.h"

#include "GGS_AssetDefinitions.generated.h"

/** Content Browser definition for the context-effects library asset. / 上下文特效库资产的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_ContextEffectsLibrary : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for context-effects preview settings. / 上下文特效预览配置的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_ContextEffectsPreviewSetting : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for interaction definitions. / 交互定义的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_InteractionDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for socket-relationship mappings. / 插槽关系映射的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_SocketRelationshipMapping : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for UI action factories. / UI Action Factory 的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_UIActionFactory : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for class-based detail-section builders. / 基于类的详情 Section Builder 内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_DetailSectionBuilder : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for GSS root setting definitions. / GSS 根设置定义的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_SettingsDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for GSS settings-list entry factories. / GSS 设置列表条目 Factory 的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_SettingsEntryWidgetFactory : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};

/** Content Browser definition for GSS settings detail-section builders. / GSS 设置详情 Section Builder 的内容浏览器定义。 */
UCLASS()
class UGGS_AssetDefinition_SettingsDetailSectionBuilder : public UAssetDefinitionDefault
{
	GENERATED_BODY()
public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};
