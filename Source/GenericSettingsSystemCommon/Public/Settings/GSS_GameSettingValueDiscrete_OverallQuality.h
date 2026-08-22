// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"

#include "GSS_GameSettingValueDiscrete_OverallQuality.generated.h"

/**
 * Overall scalability selector with an explicit Custom state.
 * 带显式“自定义”状态的综合 Scalability 选择器。
 *
 * The standard UGameUserSettings overall quality query returns a negative value when individual scalability
 * groups differ. This node presents that state as Custom instead of silently selecting an unrelated preset.
 * Video Provider wires bidirectional edit dependencies so changing a group refreshes this row immediately.
 * 标准 UGameUserSettings 的综合画质查询在各项 Scalability 不一致时会返回负值；此节点会显示“自定义”，
 * 而不是静默选择无关的预设。Video Provider 会挂上双向 EditDependency，因此改分项会立刻刷新本行。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_OverallQuality : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	virtual void OnInitialized() override;
	virtual void SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason) override;

private:
	static constexpr const TCHAR* CustomValue = TEXT("-1");
};
