// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueScalarDynamic.h"

#include "GSS_GameSettingValueScalar_Brightness.generated.h"

/**
 * Display-gamma slider backed by GEngine->DisplayGamma.
 * 由 GEngine->DisplayGamma 支持的显示 Gamma 滑条。
 *
 * Changing the slider writes `GEngine->DisplayGamma` immediately so brightness can be previewed.
 * Apply persists the current gamma under GameUserSettings.ini [GSS.Video]. The node reapplies any
 * saved value during initialization so the preference survives a restart without requiring a
 * UGameUserSettings subclass.
 * 拖动滑条会立即写入 `GEngine->DisplayGamma`，以便即时预览亮度。
 * Apply 将当前 Gamma 持久化到 GameUserSettings.ini 的 [GSS.Video]。
 * 初始化时会重新应用已保存值，因此不必继承 UGameUserSettings 也能在重启后保留该偏好。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueScalar_Brightness : public UGSS_GameSettingValueScalarDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueScalar_Brightness();

	virtual void StoreInitial() override;
	virtual double GetValue() const override;
	virtual void SetValue(double Value, EGSS_GameSettingChangeReason Reason = EGSS_GameSettingChangeReason::Change) override;

protected:
	virtual void OnInitialized() override;
	virtual void OnApply() override;

private:
	static bool TryLoadSavedGamma(float& OutGamma);
	static void SaveGamma(float Gamma);
	static void ApplyGamma(float Gamma);
};
