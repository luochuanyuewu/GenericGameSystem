// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonRotator.h"

#include "GSS_GameSettingRotator.generated.h"

/** CommonUI rotator with an optional Blueprint notification for the setting default option. / 带有可选蓝图默认项通知的 CommonUI Rotator。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick, Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_GameSettingRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	/** Updates the visual default marker without changing the selected option. / 更新视觉默认标记，不改变当前选择。 */
	void SetDefaultOption(int32 DefaultOptionIndex);

	/** Called when Refresh supplies a valid default option index. / Refresh 提供有效默认选项索引时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = Events, meta = (DisplayName = "On Default Option Specified"))
	void BP_OnDefaultOptionSpecified(int32 DefaultOptionIndex);
};
