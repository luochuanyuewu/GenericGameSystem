// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_SettingEditCondition.h"
#include "GameplayTagContainer.h"
#include "GSS_WhenPlatformHasTrait.generated.h"

class ULocalPlayer;

//////////////////////////////////////////////////////////////////////
// FGSS_WhenPlatformHasTrait

/**
 * Edit condition that evaluates CommonUI platform traits.
 * 评估 CommonUI 平台特征的编辑条件。
 *
 * Use the factory methods to hide or disable a setting when a trait is present or absent.
 * 使用工厂方法可在某个特征存在或缺失时隐藏、禁用设置。
 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_WhenPlatformHasTrait : public UGSS_SettingEditCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") FGameplayTag VisibilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") bool bRequireTrait = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") bool bHideWhenNotMatched = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") FText DisabledReason;
	virtual void Evaluate_Implementation(UGSS_SettingEditableState* InOutEditState) override;
};
