// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscrete.h"

#include "GSS_GameSettingValueDiscrete_Resolution.generated.h"

/**
 * Runtime display-resolution setting backed by UGameUserSettings.
 * 由 UGameUserSettings 支持的运行时显示分辨率设置。
 *
 * This is a custom Discrete node, not a Dynamic setting. It enumerates resolutions exposed by the
 * active RHI and rebuilds its choices whenever a registered dependency changes, normally the
 * window-mode setting. Selecting an option writes `UGameUserSettings::SetScreenResolution`
 * immediately; the subsystem commits local engine settings on Apply.
 * Serialized values use the stable `WidthxHeight` form, for example `1920x1080`.
 * 这是自定义 Discrete 节点，不是 Dynamic 设置。它会枚举当前 RHI 提供的分辨率，并在已注册依赖
 * （通常是窗口模式）变化时重建选项。选择选项会立即调用 `UGameUserSettings::SetScreenResolution`，
 * 由子系统在 Apply 时统一提交本地引擎设置。序列化值采用稳定的 `宽x高` 格式，例如 `1920x1080`。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Resolution : public UGSS_GameSettingValueDiscrete
{
	GENERATED_BODY()

public:
	/** Rebuilds options from currently available display modes. / 从当前可用显示模式重建选项。 */
	void RefreshResolutionOptions();

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	static FString ToResolutionValue(const FIntPoint& Resolution);
	virtual void OnInitialized() override;
	virtual void OnDependencyChanged() override;

private:
	void SetResolutionFromValue(const FString& Value, EGSS_GameSettingChangeReason Reason);
	int32 FindOptionIndex(const FString& Value) const;

	TArray<FString> OptionValues;
	TArray<FText> OptionDisplayTexts;
	FString InitialValue;
};
