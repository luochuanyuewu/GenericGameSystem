// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingEditCondition.h"
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
class GENERICSETTINGSSYSTEM_API UGSS_WhenPlatformHasTrait : public UGSS_GameSettingEditCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") FGameplayTag VisibilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") bool bRequireTrait = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") bool bHideWhenNotMatched = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings") FText DisabledReason;

	/** Hides the setting when the platform does not have Tag. / 平台缺少 Tag 时隐藏设置。 */
	static UGSS_WhenPlatformHasTrait* KillIfMissing(UObject* Outer, FGameplayTag Tag);
	/** Disables the setting when the platform does not have Tag. / 平台缺少 Tag 时禁用设置。 */
	static UGSS_WhenPlatformHasTrait* DisableIfMissing(UObject* Outer, FGameplayTag Tag, const FText& Reason);
	/** Hides the setting when the platform has Tag. / 平台具有 Tag 时隐藏设置。 */
	static UGSS_WhenPlatformHasTrait* KillIfPresent(UObject* Outer, FGameplayTag Tag);
	/** Disables the setting when the platform has Tag. / 平台具有 Tag 时禁用设置。 */
	static UGSS_WhenPlatformHasTrait* DisableIfPresent(UObject* Outer, FGameplayTag Tag, const FText& Reason);

	virtual void Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState) override;
};
