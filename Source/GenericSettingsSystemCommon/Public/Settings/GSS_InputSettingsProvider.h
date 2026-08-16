// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_SettingsProvider.h"

#include "GSS_InputSettingsProvider.generated.h"

/**
 * Registers common local input preferences under GSS.Settings.Input.
 * 在 GSS.Settings.Input 下注册常见的本地输入偏好。
 *
 * The provider stores input-device preferences only. The project's camera and input systems read or subscribe to
 * these tags to enact their own behavior. Gameplay policies such as aim assist belong in a project Provider.
 * 此 Provider 仅存储输入设备偏好。项目的相机和输入系统读取或订阅这些 Tag 后自行执行；
 * 瞄准辅助等玩法策略应由项目 Provider 提供。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_InputSettingsProvider : public UGSS_SettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder) override;

private:
	static FGSS_SettingValueAccessor MakeSharedAccessor(FName Getter, FName Setter);
};
