// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_GameSettingsProvider_Input.generated.h"


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
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingsProvider_Input : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	UGSS_GameSettingsProvider_Input();

	/** Default localized text registered by this Provider. Every field can be overridden by a Blueprint child. / 本 Provider 注册的默认本地化文本；每个字段都可由蓝图子类覆盖。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadAimSensitivityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadDeadZoneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadHardwareText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadInvertHorizontalText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadInvertVerticalText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadLookSensitivityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadSensitivityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText GamepadVibrationText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText InputText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText LookStickDeadZoneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MouseAndKeyboardText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MouseHorizontalSensitivityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MouseInvertHorizontalText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MouseInvertVerticalText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MouseVerticalSensitivityText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text")
	FText MoveStickDeadZoneText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText InputDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MouseHorizontalSensitivityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MouseVerticalSensitivityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MouseInvertVerticalDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MouseInvertHorizontalDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText GamepadVibrationDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText GamepadInvertVerticalDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText GamepadInvertHorizontalDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText GamepadLookSensitivityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText GamepadAimSensitivityDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText MoveStickDeadZoneDescriptionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GSS|Settings|Text", meta = (MultiLine = true))
	FText LookStickDeadZoneDescriptionText;

	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;
};
