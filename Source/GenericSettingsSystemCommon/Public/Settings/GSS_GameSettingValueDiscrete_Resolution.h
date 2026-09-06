// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GenericPlatform/GenericApplication.h"
#include "Settings/GSS_GameSettingValueDiscrete.h"

#include "GSS_GameSettingValueDiscrete_Resolution.generated.h"

struct FScreenResolutionRHI;

/**
 * Runtime display-resolution setting backed by UGameUserSettings.
 * 由 UGameUserSettings 支持的运行时显示分辨率设置。
 *
 * This is a custom Discrete node, not a Dynamic setting. It builds separate option lists for
 * Windowed, Windowed Fullscreen and exclusive Fullscreen, scoped to the currently selected monitor.
 * Selecting an option writes `UGameUserSettings::SetScreenResolution` immediately; the subsystem
 * commits local engine settings on Apply. Serialized values use the stable `WidthxHeight` form.
 * 这是自定义 Discrete 节点，不是 Dynamic 设置。它会按当前选中的显示器，分别为窗口、窗口化全屏
 * 和独占全屏构建选项列表。选择选项会立即调用 `UGameUserSettings::SetScreenResolution`，
 * 由子系统在 Apply 时统一提交本地引擎设置。序列化值采用稳定的 `宽x高` 格式。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Resolution : public UGSS_GameSettingValueDiscrete
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
	struct FScreenResolutionEntry
	{
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 RefreshRate = 0;
		FText OverrideText;

		FIntPoint GetResolution() const { return FIntPoint(Width, Height); }
		FText GetDisplayText() const;
	};

	void BindDisplayListeners();
	void UnbindDisplayListeners();
	void OnDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetrics);
	void HandleExternalDisplayChanged();
	void HandleToggleFullscreen(bool bIsFullscreen);
	void InitializeResolutions();
	void SetResolution(const FIntPoint& Resolution, EGSS_GameSettingChangeReason Reason);
	const FMonitorInfo* GetCurrentMonitor() const;
	TArrayView<const TSharedPtr<FScreenResolutionEntry>> GetSelectedResolutionList() const;
	bool ShouldAllowFullScreenResolution(const FScreenResolutionRHI& SrcScreenRes, int32 FilterThreshold) const;
	int32 FindIndexOfDisplayResolution(const FIntPoint& InPoint) const;
	int32 FindClosestResolutionIndex(const FIntPoint& Resolution) const;
	static FString ToResolutionValue(const FIntPoint& Resolution);
	static void GetStandardWindowResolutions(const FIntPoint& MinResolution, const FIntPoint& MaxResolution, float MinAspectRatio, TArray<FIntPoint>& OutResolutions);

	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsFullscreen;
	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsWindowedFullscreen;
	TArray<TSharedPtr<FScreenResolutionEntry>> ResolutionsWindowed;
	FDisplayMetrics CurrentDisplayMetrics;
	FString InitialValue;
	FDelegateHandle DisplayMetricsChangedHandle;
	FDelegateHandle WindowDisplayChangedHandle;
	FDelegateHandle ToggleFullscreenHandle;
	FDelegateHandle VideoRevertHandle;
};
