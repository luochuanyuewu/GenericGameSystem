// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_SettingEditCondition.h"
#include "GameplayTagContainer.h"
#include "GSS_WhenSettingHasValue.generated.h"

/**
 * Disables or hides a setting based on another discrete setting's pending serialized value.
 * 根据另一个离散设置的待应用序列化值，禁用或隐藏本设置。
 *
 * Matching uses the same string contract as Accessors: bool tokens and case-insensitive enum names compare equal.
 * When this condition is initialized it also registers an edit dependency on the other setting.
 * 匹配规则与 Accessor 相同：布尔记号和枚举名不区分大小写。初始化时会把另一个设置注册为编辑依赖。
 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_WhenSettingHasValue : public UGSS_SettingEditCondition
{
	GENERATED_BODY()

public:
	/** Other setting whose pending value is inspected. / 要检查其待应用值的另一个设置。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings", meta = (Categories = "GSS.Settings"))
	FGameplayTag OtherSettingId;

	/** Serialized values compared against the other setting. / 与另一个设置比较的序列化值。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	TArray<FString> MatchingValues;

	/** When false, a listed value disables this setting; when true, a value outside the list disables it. / 为 false 时，值在列表中则禁用；为 true 时，值不在列表中则禁用。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	bool bInvertMatch = false;

	/** Hide instead of disable when the match rule is satisfied. / 匹配规则成立时隐藏而不是禁用。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	bool bHideWhenMatched = false;

	/** Player-facing reason used when the setting is disabled. / 禁用时面向玩家的原因。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText DisabledReason;

	/** Disables this setting when the other setting's pending value is any of Values. / 另一个设置的待应用值为 Values 之一时禁用本设置。 */
	static UGSS_WhenSettingHasValue* DisableIfHasAnyValue(UObject* Outer, FGameplayTag OtherSettingId, TArray<FString> Values, const FText& Reason);
	/** Disables this setting when the other setting's pending value is not any of Values. / 另一个设置的待应用值不是 Values 之一时禁用本设置。 */
	static UGSS_WhenSettingHasValue* DisableIfLacksValue(UObject* Outer, FGameplayTag OtherSettingId, TArray<FString> Values, const FText& Reason);

	virtual void OnConditionInitialized_Implementation() override;
	virtual void Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState) override;
};
