// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueScalarDynamic.h"

#include "GSS_GameSettingValueScalar_Brightness.generated.h"

/**
 * Display-gamma slider backed by GEngine->DisplayGamma.
 * 由 GEngine->DisplayGamma 支持的显示 Gamma 滑条。
 *
 * Apply writes the pending gamma immediately and persists it under GameUserSettings.ini [GSS.Video].
 * The node reapplies any saved value during initialization so the preference survives a restart
 * without requiring a UGameUserSettings subclass.
 * Apply 会立即写入待应用 Gamma，并持久化到 GameUserSettings.ini 的 [GSS.Video]。
 * 初始化时会重新应用已保存值，因此不必继承 UGameUserSettings 也能在重启后保留该偏好。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueScalar_Brightness : public UGSS_GameSettingValueScalarDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueScalar_Brightness();

	virtual void StoreInitial() override;

protected:
	virtual void OnInitialized() override;
	virtual bool OnApply() override;

private:
	static bool TryLoadSavedGamma(float& OutGamma);
	static void SaveGamma(float Gamma);
	static void ApplyGamma(float Gamma);
};
