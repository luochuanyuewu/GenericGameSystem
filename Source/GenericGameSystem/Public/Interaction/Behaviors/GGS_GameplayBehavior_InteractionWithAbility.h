// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayBehavior.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GGS_GameplayBehavior_InteractionWithAbility.generated.h"

/**
 * A Gameplay Behavior for ability based interaction.
 */
UCLASS(DisplayName="GameplayBehavior_Interaction (GGS)", NotBlueprintable)
class GENERICGAMESYSTEM_API UGGS_GameplayBehavior_InteractionWithAbility : public UGameplayBehavior
{
	GENERATED_BODY()

public:
	virtual bool Trigger(AActor& InAvatar, const UGameplayBehaviorConfig* Config, AActor* SmartObjectOwner) override;
	virtual void EndBehavior(AActor& Avatar, const bool bInterrupted) override;

	bool CheckValidAbilitySetting(const UGameplayBehaviorConfig* Config, TSubclassOf<UGameplayAbility>& OutAbilityClass, int32& OutAbilityLevel);
	// the ability granted for interaction.

	UPROPERTY()
	TSubclassOf<UGameplayAbility> GrantedAbilityClass{nullptr};

	FGameplayAbilitySpecHandle AbilitySpecHandle;

	bool bBehaviorWasInterrupted = false;
	bool bAbilityEnded = false;
	bool bAbilityWasCancelled = false;

	FDelegateHandle AbilityEndedDelegateHandle;


	virtual void OnAbilityEndedCallback(const FAbilityEndedData& EndedData);
};
