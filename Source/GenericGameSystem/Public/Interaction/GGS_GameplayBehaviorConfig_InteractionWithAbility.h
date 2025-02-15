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
UCLASS()
class GENERICGAMESYSTEM_API UGGS_GameplayBehaviorConfig_InteractionWithAbility : public UGameplayBehaviorConfig
{
	GENERATED_BODY()

public:

	UGGS_GameplayBehaviorConfig_InteractionWithAbility();
	
	/**
	 * The ability to grant and activate when interaction begin.
	 * The ability has to be ServerOnly/ServerInitiated, and triggered from event.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Interaction")
	TSoftClassPtr<UGameplayAbility> AbilityToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int32 AbilityLevel{0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FGameplayTag AbilityTriggeringTag;

	/** The widget to show for this kind of interaction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;

#if WITH_EDITORONLY_DATA
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
