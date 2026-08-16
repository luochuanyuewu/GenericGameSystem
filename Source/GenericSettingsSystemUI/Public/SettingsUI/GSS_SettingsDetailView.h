// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "UI/Common/GUIS_ListEntryDetailView.h"

#include "GSS_SettingsDetailView.generated.h"

class UCommonRichTextBlock;
class UCommonTextBlock;
class UGSS_GameSetting;
enum class EGSS_GameSettingChangeReason : uint8;

/**
 * Standard GSS detail view with optional title, description, warning and availability fields.
 * 带有可选标题、说明、警告和可用性字段的标准 GSS 详情视图。
 *
 * This keeps the GUIS detail-section builder, async loading and widget pool. Bind only the fields a project
 * needs in a Blueprint subclass; SectionsBuilder may additionally add UGSS_SettingsDetailSection widgets.
 * 本类保留 GUIS 的详情 Section Builder、异步加载和 Widget Pool。蓝图子类只需绑定项目需要的字段；
 * SectionsBuilder 还可额外添加 UGSS_SettingsDetailSection Widget。
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsDetailView : public UGUIS_ListEntryDetailView
{
	GENERATED_BODY()

public:
	UGSS_SettingsDetailView(const FObjectInitializer& ObjectInitializer);

	/** Updates the standard fields and GUIS Sections for a setting. / 更新一个设置的标准字段与 GUIS Section。 */
	void FillSettingDetails(UGSS_GameSetting* InSetting);
	/** Assigns the setting and refreshes standard details and GUIS Sections. / 分配设置，并刷新标准详情和 GUIS Section。 */
	virtual void SetListItemObject(UObject* InListItemObject) override;
	/** Returns the setting currently represented by this view. / 返回此视图当前表示的设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_GameSetting* GetSetting() const { return CurrentSetting; }

	virtual void NativeDestruct() override;

	/** Called after the view has refreshed its standard detail fields. / 视图刷新标准详情字段后调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingDetailsUpdated(UGSS_GameSetting* Setting);

private:
	void ReleaseCurrentSetting();
	void HandleCurrentSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason);
	void HandleCurrentSettingEditableStateChanged(UGSS_GameSetting* ChangedSetting);
	void RefreshSettingDetails();

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSetting> CurrentSetting;

	/** Optional title text. / 可选标题文本。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonTextBlock> Text_SettingName;
	/** Optional rich-text description. / 可选富文本说明。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonRichTextBlock> RichText_Description;
	/** Optional contextual dynamic details. / 可选上下文动态详情。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonRichTextBlock> RichText_DynamicDetails;
	/** Optional setting warning text. / 可选设置警告文本。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonRichTextBlock> RichText_WarningDetails;
	/** Optional disabled-state and unavailable-option explanations. / 可选禁用状态与不可用选项说明。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UCommonRichTextBlock> RichText_DisabledDetails;
};
