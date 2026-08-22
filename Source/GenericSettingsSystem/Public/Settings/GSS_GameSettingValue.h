// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"

#include "GSS_GameSettingValue.generated.h"

class UObject;

//--------------------------------------
// UGSS_GameSettingValue
//--------------------------------------

/**
 * Base class for editable setting values with transactional Apply/Cancel support.
 * 支持事务式 Apply/Cancel 的可编辑设置值基类。
 *
 * Dynamic implementations write their Accessor immediately so values can preview live. Apply/Cancel
 * still persist or restore the last captured initial value. Custom Discrete nodes such as Language
 * may stage a value until Apply.
 * Dynamic 实现会立即写入 Accessor，以便即时预览。Apply/Cancel 仍负责持久化或恢复上次记录的初始值。
 * Language 等自定义 Discrete 节点可以在 Apply 前暂存值。
 */
UCLASS(Abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValue : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValue();

	/** Captures the current applied value as the restore target. / 将当前已应用值记录为恢复目标。 */
	virtual void StoreInitial() PURE_VIRTUAL(, );

	/** Restores the value to its definition default. / 将值恢复为 Definition 默认值。 */
	virtual void ResetToDefault() PURE_VIRTUAL(, );

	/** Restores the value to the last captured initial value. / 将值恢复为最后记录的初始值。 */
	virtual void RestoreToInitial() PURE_VIRTUAL(, );
protected:
	virtual void OnInitialized() override;
};
