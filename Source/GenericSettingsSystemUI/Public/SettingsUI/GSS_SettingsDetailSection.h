// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "UI/Common/GUIS_ListEntryDetailSection.h"

#include "GSS_SettingsDetailSection.generated.h"

class UGSS_GameSetting;
enum class EGSS_GameSettingChangeReason : uint8;

/**
 * Blueprint base for a detail section that presents one GSS runtime setting.
 * 用于展示一个 GSS 运行时设置的详情 Section 蓝图基类。
 *
 * Configure a GUIS detail-section builder to map UGSS_GameSetting or a derived setting class to a
 * Blueprint derived from this class. The section automatically follows the detail view's current setting
 * and forwards value and edit-state updates to Blueprint.
 * 在 GUIS 详情 Section Builder 中，将 UGSS_GameSetting 或其派生设置类映射到本类的蓝图子类。
 * 本 Section 会自动跟随详情视图的当前设置，并将数值和编辑状态更新转发给蓝图。
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsDetailSection : public UGUIS_ListEntryDetailSection
{
	GENERATED_BODY()

public:
	/** Returns the runtime setting currently displayed by this section. / 返回此 Section 当前展示的运行时设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_GameSetting* GetSetting() const { return Setting; }

	virtual void NativeDestruct() override;

	/** Called whenever this section receives a setting, including nullptr when the detail view is cleared. / 每次本 Section 接收到设置时调用；详情视图清空时参数为 nullptr。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingAssigned(UGSS_GameSetting* AssignedSetting);
	/** Called when the displayed setting's pending value or applied value changes. / 展示设置的待应用值或已应用值变化时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingValueChanged();
	/** Called when the displayed setting's visibility, availability or option state changes. / 展示设置的可见性、可用性或选项状态变化时调用。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GSS|Settings UI")
	void OnSettingEditableStateChanged();

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	void ReleaseSetting();
	void HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason);
	void HandleEditStateChanged(UGSS_GameSetting* ChangedSetting);

	/** Runtime setting represented by this detail section. / 此详情 Section 表示的运行时设置。 */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings UI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UGSS_GameSetting> Setting;
};
