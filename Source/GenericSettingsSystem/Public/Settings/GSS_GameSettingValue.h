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
 * Implementations keep an initial applied value and a pending editable value; game logic can read the pending
 * value for immediate preview, while Accessors are written only by Apply.
 * 实现类保存初始已应用值和待应用编辑值；游戏逻辑可读取待应用值进行即时预览，而 Accessor 只会在 Apply 时写入。
 */
UCLASS(Abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValue : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValue();

	/** Captures the current applied value as the restore target. / 将当前已应用值记录为恢复目标。 */
	virtual void StoreInitial() PURE_VIRTUAL(, );

	/** Changes the pending value to its definition default. / 将待应用值改为 Definition 默认值。 */
	virtual void ResetToDefault() PURE_VIRTUAL(, );

	/** Restores the pending value to the last captured applied value. / 将待应用值恢复为最后记录的已应用值。 */
	virtual void RestoreToInitial() PURE_VIRTUAL(, );
protected:
	virtual void OnInitialized() override;
};
