// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"

#include "GSS_GameSettingValueDiscrete_Display.generated.h"

/**
 * Runtime display selector that enumerates the operating system's monitors.
 * 枚举操作系统显示器的运行时显示目标选择器。
 *
 * Monitor IDs and labels are obtained from FDisplayMetrics. Supply a project Accessor that reads and applies the
 * selected monitor ID, because stock UGameUserSettings has no cross-platform monitor-selection API.
 * 监视器 ID 与标签来自 FDisplayMetrics。必须提供项目 Accessor 来读写选中的监视器 ID，
 * 因为原生 UGameUserSettings 没有跨平台的显示器选择 API。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Display : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	/** Rebuilds monitor ID options from current display metrics. / 从当前显示器指标重建监视器 ID 选项。 */
	void RefreshDisplayOptions();

protected:
	virtual void OnInitialized() override;
	virtual void OnDependencyChanged() override;
};
