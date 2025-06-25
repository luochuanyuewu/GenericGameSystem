// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SmartObjectRuntime.h"
#include "SmartObjectTypes.h"
#include "GGS_AbilityTask_UseSmartObjectWithGameplayBehavior.generated.h"

class UGameplayBehavior;
/**
 * 
 */
UCLASS()
class GENERICGAMESYSTEM_API UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior : public UAbilityTask
{
	GENERATED_BODY()

public:
	UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UFUNCTION(BlueprintCallable, Category = "GGS|Interaction", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior* UseSmartObjectWithGameplayBehavior(UGameplayAbility* OwningAbility, FSmartObjectClaimHandle ClaimHandle,
	                                                                                               ESmartObjectClaimPriority ClaimPriority = ESmartObjectClaimPriority::Normal);

	void SetClaimHandle(const FSmartObjectClaimHandle& Handle) { ClaimedHandle = Handle; }

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	bool StartInteraction();

	void OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted);

	// called when slot invalid(actor destoryed etc..)
	void OnSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State);

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnSucceeded;

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnFailed;

	UPROPERTY()
	TObjectPtr<UGameplayBehavior> GameplayBehavior;

	FSmartObjectClaimHandle ClaimedHandle;
	FDelegateHandle OnBehaviorFinishedNotifyHandle;

	bool bBehaviorFinished;
};
