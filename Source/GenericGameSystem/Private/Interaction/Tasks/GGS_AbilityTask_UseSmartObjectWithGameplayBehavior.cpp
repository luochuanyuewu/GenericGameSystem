// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.


#include "Interaction/Tasks/GGS_AbilityTask_UseSmartObjectWithGameplayBehavior.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameplayBehavior.h"
#include "GameplayBehaviorConfig.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "GameplayBehaviorSubsystem.h"
#include "GGS_LogChannels.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"

UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bBehaviorFinished = false;
}

UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior* UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::UseSmartObjectWithGameplayBehavior(UGameplayAbility* OwningAbility,
	FSmartObjectClaimHandle ClaimHandle, ESmartObjectClaimPriority ClaimPriority)
{
	if (OwningAbility == nullptr)
	{
		return nullptr;
	}


	UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior* MyTask = NewAbilityTask<UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior>(OwningAbility);
	if (MyTask == nullptr)
	{
		return nullptr;
	}
	MyTask->SetClaimHandle(ClaimHandle);
	return MyTask;
}

void UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::Activate()
{
	Super::Activate();
	bool bSuccess = false;
	ON_SCOPE_EXIT
	{
		if (!bSuccess)
		{
			EndTask();
		}
	};

	if (!ensureMsgf(ClaimedHandle.IsValid(), TEXT("SmartObject handle must be valid at this point.")))
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(GetAvatarActor());
	if (Pawn == nullptr)
	{
		GGS_CLOG(Error, "Pawn required to use GameplayBehavior with claim handle: %s.", *LexToString(ClaimedHandle));
		return;
	}
	USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(Pawn->GetWorld());
	if (!ensureMsgf(SmartObjectSubsystem != nullptr, TEXT("SmartObjectSubsystem must be accessible at this point.")))
	{
		return;
	}

	// Claim handle validity and simulation validity are different checks; both are required.
	// Claim 句柄“格式有效”不等于“仍在仿真中有效”，两者都必须检查。
	if (!SmartObjectSubsystem->IsClaimedSmartObjectValid(ClaimedHandle))
	{
		GGS_CLOG(Log, "Claim handle: %s refers to an object that is no longer available.", *LexToString(ClaimedHandle));
		return;
	}

	// Keep listening for slot invalidation so task can abort immediately on ownership loss.
	// 持续监听槽位失效，确保占用权丢失时任务立即中止。
	SmartObjectSubsystem->RegisterSlotInvalidationCallback(ClaimedHandle, FOnSlotInvalidated::CreateUObject(this, &UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSlotInvalidated));

	bSuccess = StartInteraction();
}

bool UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::StartInteraction()
{
	UWorld* World = GetWorld();
	USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(World);
	if (!ensure(SmartObjectSubsystem))
	{
		return false;
	}

	const UGameplayBehaviorSmartObjectBehaviorDefinition* SmartObjectGameplayBehaviorDefinition = SmartObjectSubsystem->MarkSlotAsOccupied<
		UGameplayBehaviorSmartObjectBehaviorDefinition>(ClaimedHandle);
	const UGameplayBehaviorConfig* GameplayBehaviorConfig = SmartObjectGameplayBehaviorDefinition != nullptr ? SmartObjectGameplayBehaviorDefinition->GameplayBehaviorConfig : nullptr;
	GameplayBehavior = GameplayBehaviorConfig != nullptr ? GameplayBehaviorConfig->GetBehavior(*World) : nullptr;
	if (GameplayBehavior == nullptr)
	{
		return false;
	}

	const USmartObjectComponent* SmartObjectComponent = SmartObjectSubsystem->GetSmartObjectComponent(ClaimedHandle);
	AActor& InteractorActor = *GetAvatarActor();
	AActor* InteracteeActor = SmartObjectComponent ? SmartObjectComponent->GetOwner() : nullptr;
	const bool bBehaviorActive = UGameplayBehaviorSubsystem::TriggerBehavior(*GameplayBehavior, InteractorActor, GameplayBehaviorConfig, InteracteeActor);
	// Behavior can complete synchronously on trigger; callback is only needed for still-running behaviors.
	// 行为可能在触发当帧同步结束；仅对仍在运行的行为注册完成回调。
	if (bBehaviorActive)
	{
		OnBehaviorFinishedNotifyHandle = GameplayBehavior->GetOnBehaviorFinishedDelegate().AddUObject(this, &UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSmartObjectBehaviorFinished);
	}

	return bBehaviorActive;
}

void UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSmartObjectBehaviorFinished(UGameplayBehavior& Behavior, AActor& Avatar, const bool bInterrupted)
{
	// Adding an ensure in case the assumptions change in the future.
	ensure(GetAvatarActor() != nullptr);

	// make sure we handle the right pawn - we can get this notify for a different
	// Avatar if the behavior sending it out is not instanced (CDO is being used to perform actions)
	if (GetAvatarActor() == &Avatar)
	{
		Behavior.GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		bBehaviorFinished = true;
		EndTask();
	}
}

void UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::OnDestroy(bool bInOwnerFinished)
{
	if (ClaimedHandle.IsValid())
	{
		USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(GetWorld());
		check(SmartObjectSubsystem);
		// Always release slot on task teardown to avoid leaked occupancy across ability cancellation paths.
		// 任务结束时始终释放槽位，避免技能取消路径留下占用泄漏。
		SmartObjectSubsystem->MarkSlotAsFree(ClaimedHandle);
		SmartObjectSubsystem->UnregisterSlotInvalidationCallback(ClaimedHandle);
		ClaimedHandle.Invalidate();
	}

	if (TaskState != EGameplayTaskState::Finished)
	{
		if (GameplayBehavior != nullptr && bBehaviorFinished)
		{
			OnSucceeded.Broadcast();
		}
		else
		{
			OnFailed.Broadcast();
		}
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UGGS_AbilityTask_UseSmartObjectWithGameplayBehavior::OnSlotInvalidated(const FSmartObjectClaimHandle& ClaimHandle, const ESmartObjectSlotState State)
{
	if (!bBehaviorFinished && GameplayBehavior != nullptr)
	{
		check(GetAvatarActor());
		GameplayBehavior->GetOnBehaviorFinishedDelegate().Remove(OnBehaviorFinishedNotifyHandle);
		GameplayBehavior->AbortBehavior(*GetAvatarActor());
	}
	EndTask();
}
