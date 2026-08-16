// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"

#include "GSS_GameSettingValueDiscrete_Resolution.generated.h"

/**
 * Runtime display-resolution setting backed by UGameUserSettings.
 * 由 UGameUserSettings 支持的运行时显示分辨率设置。
 *
 * Unlike a static Builder discrete setting, this node enumerates resolutions exposed by the active RHI and rebuilds
 * its choices whenever a registered dependency changes, normally the window-mode setting. It directly owns the
 * UGameUserSettings read/write behavior. Serialized values use the stable `WidthxHeight` form, for example `1920x1080`.
 * 与静态 Builder 离散设置不同，此节点会枚举当前 RHI 提供的分辨率，并在已注册依赖（通常是窗口模式）变化时重建选项。
 * 序列化值采用稳定的 `宽x高` 格式，例如 `1920x1080`。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Resolution : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	/** Rebuilds options from currently available display modes. / 从当前可用显示模式重建选项。 */
	void RefreshResolutionOptions();
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

protected:
	static FString ToResolutionValue(const FIntPoint& Resolution);
	virtual void OnInitialized() override;
	virtual void OnDependencyChanged() override;
	virtual bool OnApply() override;
};
