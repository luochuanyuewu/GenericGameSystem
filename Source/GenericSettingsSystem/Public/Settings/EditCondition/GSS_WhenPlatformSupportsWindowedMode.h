// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingEditCondition.h"
#include "GSS_WhenPlatformSupportsWindowedMode.generated.h"

/**
 * Hides a setting on platforms that cannot use windowed or windowed-fullscreen display modes.
 * 在不支持窗口化或窗口化全屏显示模式的平台上隐藏设置。
 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_WhenPlatformSupportsWindowedMode : public UGSS_GameSettingEditCondition
{
	GENERATED_BODY()

public:
	/** Creates a condition owned by Outer, normally the setting it will be added to. / 创建由 Outer 持有的条件，通常是即将添加该条件的设置。 */
	static UGSS_WhenPlatformSupportsWindowedMode* Create(UObject* Outer);
	virtual void Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState) override;
};
