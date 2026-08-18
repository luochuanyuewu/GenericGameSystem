// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "SettingsUI/GSS_SettingsListEntry.h"

#include "GSS_SettingsValueEntries.generated.h"

class UAnalogSlider;
class UCommonButtonBase;
class UCommonTextBlock;
class UGSS_GameSettingAction;
class UGSS_GameSettingCollectionPage;
class UGSS_GameSettingValueDiscrete;
class UGSS_GameSettingValueScalar;
class UGSS_SettingsRotator;
class UPanelWidget;

/** Blueprint base for a discrete (including bool) GSS setting row. / 离散值（包括布尔值）GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick, Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Discrete : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<FText> GetOptions() const;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	int32 GetSelectedOptionIndex() const;
	/** Returns the default index in the currently visible option list, or INDEX_NONE. / 返回当前可见选项列表中的默认索引；没有默认值时为 INDEX_NONE。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	int32 GetDefaultOptionIndex() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool SelectOption(int32 OptionIndex);

protected:
	virtual void HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason) override;
	virtual void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting) override;
	void RefreshDiscreteControl();
	void HandleOptionDecrease();
	void HandleOptionIncrease();
	void HandleRotatorChangedValue(int32 Value, bool bUserInitiated);

	/** Assigned discrete setting cached for native controls. / 为原生控件缓存的已分配离散设置。 */
	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingValueDiscrete> DiscreteSetting;

	/** Optional container for the discrete value controls. / 离散值控件的可选容器。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UPanelWidget> Panel_Value;
	/** Optional CommonUI selector for discrete options. / 离散选项的可选 CommonUI 选择器。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UGSS_SettingsRotator> Rotator_SettingValue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonButtonBase> Button_Decrease;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonButtonBase> Button_Increase;
};

/** Blueprint base for a scalar GSS setting row. / 标量 GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Scalar : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	double GetNormalizedValue() const;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetFormattedValue() const;
	/** Returns the default value in normalized UI space when one exists. / 有默认值时，返回归一化 UI 空间中的默认值。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	bool GetDefaultNormalizedValue(double& OutValue) const;
	/** Returns the normalized slider step size. / 返回归一化滑块步长。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	double GetNormalizedStepSize() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool SetNormalizedValue(double Value);

protected:
	virtual void HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason) override;
	virtual void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting) override;
	void RefreshScalarControl();
	UFUNCTION() void HandleSliderValueChanged(float Value);
	UFUNCTION() void HandleSliderCaptureEnded();

	/** Called after the native slider has refreshed its normalized value. / 原生滑块刷新归一化值后调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnScalarValueChanged(float Value);
	/** Called after the native slider has refreshed its default value. / 原生滑块刷新默认值后调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnScalarDefaultValueChanged(float DefaultValue, bool bHasDefaultValue);

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingValueScalar> ScalarSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UPanelWidget> Panel_Value;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UAnalogSlider> Slider_SettingValue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonTextBlock> Text_SettingValue;
};

/** Blueprint base for a command GSS setting row. / 命令型 GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Action : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetActionText() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool ExecuteAction();

protected:
	virtual void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting) override;
	void HandleActionButtonClicked();
	/** Called when the row receives an Action setting, so Blueprint can populate Button_Action text. / 行接收 Action 设置时调用，供蓝图填充 Button_Action 文本。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnActionSettingAssigned(const FText& ActionText);
	UPROPERTY(Transient) TObjectPtr<UGSS_GameSettingAction> ActionSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI") TObjectPtr<UCommonButtonBase> Button_Action;
};

/** Blueprint base for a navigable GSS settings page row. / 可导航 GSS 设置页面行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Navigation : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetNavigationText() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool Navigate();

protected:
	virtual void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting) override;
	void HandleNavigationButtonClicked();
	/** Called when the row receives a page setting, so Blueprint can populate Button_Navigate text. / 行接收页面设置时调用，供蓝图填充 Button_Navigate 文本。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnNavigationSettingAssigned(const FText& NavigationText);
	UPROPERTY(Transient) TObjectPtr<UGSS_GameSettingCollectionPage> CollectionSetting;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI") TObjectPtr<UCommonButtonBase> Button_Navigate;
};
