// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/** Editor integrations for Generic Game System assets. / Generic Game System 资产的编辑器集成。 */
class GENERICGAMESYSTEMEDITOR_API FGenericGameSystemEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	/** Returns the Content Browser category used by GGS asset factories. / 返回 GGS 资产 Factory 使用的内容浏览器分类。 */
	static uint32 GetAssetCategory() { return AssetCategory; }

private:
	static uint32 AssetCategory;
};
