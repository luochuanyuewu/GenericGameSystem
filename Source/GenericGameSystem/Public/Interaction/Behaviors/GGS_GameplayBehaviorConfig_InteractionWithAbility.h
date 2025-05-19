// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GGS_GameplayBehaviorConfig_InteractionWithAbility.generated.h"

class UGameplayAbility;
class UUserWidget;

/**
 * A Gameplay Behavior Config for ability based interaction.
 */
UCLASS(DisplayName="Gameplay Bheavior Config Interaction (GGS)")
class GENERICGAMESYSTEM_API UGGS_GameplayBehaviorConfig_InteractionWithAbility : public UGameplayBehaviorConfig
{
	GENERATED_BODY()

public:
	UGGS_GameplayBehaviorConfig_InteractionWithAbility();

	/**
	 * The ability to grant and activate when interaction begins.
	 * @attention Must be Instanced Ability, Must not be LocalOnly. Doesn't support event triggered ability.
	 * 交互开始后会赋予并执行的技能。
	 * @注意 必须是实例化的技能，不能是LocalOnly。不支持通过事件触发的技能。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TSoftClassPtr<UGameplayAbility> AbilityToGrant;

	/**
	 * The ability level.
	 * @details You can use levels to make distinctions in visual, e.g. 0~10 each corresponds to a different pickup visual effect.
	 * 交互技能的等级。
	 * @细节 你可以使用等级来做表现上的区分，比如0~10分别对应不同的拾取视觉效果。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 AbilityLevel{0};

	// /** The widget to show for this kind of interaction. */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	// TSoftClassPtr<UUserWidget> InteractionWidgetClass;

#if WITH_EDITORONLY_DATA
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
