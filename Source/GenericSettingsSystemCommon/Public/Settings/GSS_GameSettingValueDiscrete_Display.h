// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GenericPlatform/GenericApplication.h"
#include "Settings/GSS_GameSettingValueDiscrete.h"

#include "GSS_GameSettingValueDiscrete_Display.generated.h"

/**
 * Runtime monitor-selection setting backed by UGameUserSettings display properties.
 * 由 UGameUserSettings 显示器属性支持的运行时显示器选择设置。
 *
 * Enumerates connected monitors from Slate display metrics and writes
 * `UGameUserSettings::SetDisplayProperties` immediately. The subsystem commits the window on Apply.
 * 从 Slate 显示度量枚举已连接显示器，并立即调用 `UGameUserSettings::SetDisplayProperties`。
 * 由子系统在 Apply 时提交窗口。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Display : public UGSS_GameSettingValueDiscrete
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual int32 GetDiscreteOptionDefaultIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	virtual void OnInitialized() override;
	virtual void OnDependencyChanged() override;

private:
	void BindDisplayListeners();
	void UnbindDisplayListeners();
	void OnDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetrics);
	void HandleExternalDisplayChanged();
	void HandleToggleFullscreen(bool bIsFullscreen);
	void SetDisplayFromIndex(int32 Index, EGSS_GameSettingChangeReason Reason);

	FString InitialMonitorID;
	int32 InitialMonitorIndex = INDEX_NONE;
	FDisplayMetrics CurrentDisplayMetrics;
	FDelegateHandle DisplayMetricsChangedHandle;
	FDelegateHandle WindowDisplayChangedHandle;
	FDelegateHandle ToggleFullscreenHandle;
	FDelegateHandle VideoRevertHandle;
};
