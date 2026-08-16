// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GSS_SettingEditCondition.generated.h"

class UGSS_GameSetting;

/** Mutable result of edit-condition evaluation. / 编辑条件评估的可变结果。 */
UCLASS(BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_SettingEditableState : public UObject
{
	GENERATED_BODY()

public:
	void Reset();
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") bool IsVisible() const { return bVisible; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") bool IsEnabled() const { return bEnabled; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") bool IsResettable() const { return bResettable; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") bool IsHiddenFromAnalytics() const { return bHiddenFromAnalytics; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") TArray<FText> GetDisabledReasons() const { return DisabledReasons; }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") TArray<FString> GetDisabledOptions() const { return DisabledOptions; }

	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void Hide(const FString& DeveloperReason);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void Disable(const FText& Reason);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void DisableOption(const FString& OptionValue);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void PreventReset();
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void HideFromAnalytics();
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void Kill(const FString& DeveloperReason);

private:
	bool bVisible = true;
	bool bEnabled = true;
	bool bResettable = true;
	bool bHiddenFromAnalytics = false;
	TArray<FString> DisabledOptions;
	TArray<FText> DisabledReasons;
};

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
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings") void Evaluate(UGSS_SettingEditableState* InOutState);
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings") void RequestRefresh();
	UFUNCTION(BlueprintPure, Category = "GSS|Settings") UGSS_GameSetting* GetSetting() const { return Setting; }

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GSS|Settings")
	TObjectPtr<UGSS_GameSetting> Setting;
};

/** C++-only inline edit condition. / 仅供 C++ 使用的内联编辑条件。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_InlineSettingEditCondition : public UGSS_SettingEditCondition
{
	GENERATED_BODY()
public:
	using FEvaluator = TFunction<void(UGSS_GameSetting*, UGSS_SettingEditableState*)>;
	void SetEvaluator(FEvaluator&& InEvaluator) { Evaluator = MoveTemp(InEvaluator); }
	virtual void Evaluate_Implementation(UGSS_SettingEditableState* InOutState) override { if (Evaluator) Evaluator(Setting, InOutState); }
private:
	FEvaluator Evaluator;
};
