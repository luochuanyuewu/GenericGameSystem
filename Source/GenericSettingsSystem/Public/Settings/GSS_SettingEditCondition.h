// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingFilterState.h"
#include "GSS_SettingEditCondition.generated.h"

class UGSS_GameSetting;

/** Blueprint/C++ rule that contributes a setting's editable state. / 为设置可编辑状态提供规则的蓝图/C++对象。 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GENERICSETTINGSSYSTEM_API UGSS_SettingEditCondition : public UObject
{
	GENERATED_BODY()

public:
	void InitializeCondition(UGSS_GameSetting* InSetting);
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings") void OnConditionInitialized();
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings") void OnSettingValueChanged(EGSS_GameSettingChangeReason Reason);
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings") void OnSettingApplied();
	/** Evaluates and mutates the owning setting's current state. / 评估并修改所属设置的当前状态。 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings") void Evaluate(UPARAM(ref) FGSS_GameSettingEditableState& InOutState);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void RequestRefresh();
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") UGSS_GameSetting* GetSetting() const { return Setting; }

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings")
	TObjectPtr<UGSS_GameSetting> Setting;
};
