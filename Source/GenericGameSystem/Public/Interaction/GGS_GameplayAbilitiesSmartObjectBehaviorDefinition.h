// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SmartObjectDefinition.h"
#include "Engine/DataTable.h"
#include "GGS_GameplayAbilitiesSmartObjectBehaviorDefinition.generated.h"

class UGameplayAbility;
class UUserWidget;

/**
 * SmartObject behavior definition for the GameplayAbilities framework
 */
UCLASS()
class GENERICGAMESYSTEM_API UGGS_GameplayAbilitiesSmartObjectBehaviorDefinition : public USmartObjectBehaviorDefinition
{
	GENERATED_BODY()

public:
	/** Simple text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FText SubText;

	/**
	 * Which CommonUI input to trigger this interaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle TriggeringInputAction;

	/** The ability to grant the avatar when they get near interactable objects. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TSoftClassPtr<UGameplayAbility> AbilityToGrant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	int32 AbilityLevel{0};

	/** The widget to show for this kind of interaction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TSoftClassPtr<UUserWidget> IndicatorWidgetClass;
};
