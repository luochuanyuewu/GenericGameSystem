// Copyright 2025 https://yuewu.dev/en  All Rights Reserved.


#include "Ragdoll/GGS_RagdollComponent.h"
#include "GGS_LogChannels.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkinnedAsset.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values for this component's properties
UGGS_RagdollComponent::UGGS_RagdollComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

const FGGS_RagdollState& UGGS_RagdollComponent::GetRagdollState() const
{
	return RagdollState;
}

bool UGGS_RagdollComponent::IsRagdollAllowedToStart() const
{
	if (!IsValid(MeshComponent))
	{
		GGS_CLOG(Warning, "Missing skeletal mesh component for the Ragdoll to work.")
		return false;
	}
	if (bRagdolling)
	{
		return false;
	}
	FBodyInstance* PelvisBodyInstance = MeshComponent->GetBodyInstance(PelvisBoneName);
	FBodyInstance* SpineBodyInstance = MeshComponent->GetBodyInstance(SpineBoneName);
	if (PelvisBodyInstance == nullptr || SpineBodyInstance == nullptr)
	{
		GGS_CLOG(Warning, "A physics asset with the %s and %s bones are required for the Ragdoll to work.(Also ensure mesh component has collision enabled)", *PelvisBoneName.ToString(),
		         *SpineBoneName.ToString())
		return false;
	}
	return true;
}

void UGGS_RagdollComponent::SetMeshComponent_Implementation(USkeletalMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}

bool UGGS_RagdollComponent::IsRagdolling_Implementation() const
{
	return bRagdolling;
}

void UGGS_RagdollComponent::StartRagdoll()
{
	if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy || !IsRagdollAllowedToStart())
	{
		return;
	}

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		MulticastStartRagdoll();
	}
	else
	{
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			Character->GetCharacterMovement()->FlushServerMoves();
		}
		ServerStartRagdoll();
	}
}

void UGGS_RagdollComponent::ServerStartRagdoll_Implementation()
{
	if (IsRagdollAllowedToStart())
	{
		MulticastStartRagdoll();
		GetOwner()->ForceNetUpdate();
	}
}

void UGGS_RagdollComponent::MulticastStartRagdoll_Implementation()
{
	LocalStartRagdoll();
}

void UGGS_RagdollComponent::LocalStartRagdoll()
{
	if (!IsRagdollAllowedToStart())
	{
		return;
	}

	MeshComponent->bUpdateJointsFromAnimation = true; // Required for the flail animation to work properly.

	if (!MeshComponent->IsRunningParallelEvaluation() && !MeshComponent->GetBoneSpaceTransforms().IsEmpty())
	{
		MeshComponent->UpdateRBJointMotors();
	}

	// Stop any active montages.

	static constexpr auto BlendOutDuration{0.2f};

	MeshComponent->GetAnimInstance()->Montage_Stop(BlendOutDuration);

	if (IsValid(CharacterOwner))
	{
		// Disable movement corrections and reset network smoothing.
		CharacterOwner->GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Disabled;
		CharacterOwner->GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
	}

	// Detach the mesh so that character transformation changes will not affect it in any way.

	MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	// Disable capsule collision and enable mesh physics simulation.

	UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	{
		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetSimulatePhysics(true);

	// This is required for the ragdoll to behave properly when any body instance is set to simulated in a physics asset.
	// TODO Check the need for this in future engine versions.
	MeshComponent->ResetAllBodiesSimulatePhysics();

	const auto* PelvisBody{MeshComponent->GetBodyInstance(PelvisBoneName)};
	FVector PelvisLocation;

	FPhysicsCommand::ExecuteRead(PelvisBody->ActorHandle, [this, &PelvisLocation](const FPhysicsActorHandle& ActorHandle)
	{
		PelvisLocation = FPhysicsInterface::GetTransform_AssumesLocked(ActorHandle, true).GetLocation();
		RagdollState.Velocity = FPhysicsInterface::GetLinearVelocity_AssumesLocked(ActorHandle);
	});

	RagdollState.PullForce = 0.0f;

	if (bLimitInitialRagdollSpeed)
	{
		// Limit the ragdoll's speed for a few frames, because for some unclear reason,
		// it can get a much higher initial speed than the character's last speed.

		// TODO Find a better solution or wait for a fix in future engine versions.

		static constexpr auto MinSpeedLimit{200.0f};

		RagdollState.SpeedLimitFrameTimeRemaining = 8;
		RagdollState.SpeedLimit = FMath::Max(MinSpeedLimit, UE_REAL_TO_FLOAT(GetOwner()->GetVelocity().Size()));

		ConstraintRagdollSpeed();
	}

	if (PawnOwner->GetLocalRole() >= ROLE_Authority)
	{
		SetRagdollTargetLocation(FVector::ZeroVector);
	}

	if (PawnOwner->IsLocallyControlled() || (PawnOwner->GetLocalRole() >= ROLE_Authority && !IsValid(PawnOwner->GetController())))
	{
		SetRagdollTargetLocation(PelvisLocation);
	}

	// Clear the character movement mode and set the locomotion action to Ragdoll.

	if (IsValid(CharacterOwner))
	{
		CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
	bRagdolling = true;
	OnRagdollStarted();
}

void UGGS_RagdollComponent::OnRagdollStarted_Implementation()
{
	OnRagdollStartedEvent.Broadcast();
}

bool UGGS_RagdollComponent::IsRagdollAllowedToStop() const
{
	return bRagdolling;
}

bool UGGS_RagdollComponent::StopRagdoll()
{
	if (GetOwner()->GetLocalRole() <= ROLE_SimulatedProxy || !IsRagdollAllowedToStop())
	{
		return false;
	}

	if (GetOwner()->GetLocalRole() >= ROLE_Authority)
	{
		MulticastStopRagdoll();
	}
	else
	{
		ServerStopRagdoll();
	}

	return true;
}

void UGGS_RagdollComponent::ServerStopRagdoll_Implementation()
{
	if (IsRagdollAllowedToStop())
	{
		MulticastStopRagdoll();
		GetOwner()->ForceNetUpdate();
	}
}

void UGGS_RagdollComponent::MulticastStopRagdoll_Implementation()
{
	LocalStopRagdoll();
}

void UGGS_RagdollComponent::LocalStopRagdoll()
{
	if (!IsRagdollAllowedToStop())
	{
		return;
	}

	MeshComponent->SnapshotPose(RagdollState.FinalRagdollPose);

	const auto PelvisTransform{MeshComponent->GetSocketTransform(PelvisBoneName)};
	const auto PelvisRotation{PelvisTransform.Rotator()};

	// Disable mesh physics simulation and enable capsule collision.

	MeshComponent->bUpdateJointsFromAnimation = false;

	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);

	UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	{
		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}


	bool bGrounded;
	const auto NewActorLocation{RagdollTraceGround(bGrounded)};

	// Determine whether the ragdoll is facing upward or downward and set the actor rotation accordingly.

	const auto bRagdollFacingUpward{FMath::UnwindDegrees(PelvisRotation.Roll) <= 0.0f};

	auto NewActorRotation{GetOwner()->GetActorRotation()};
	NewActorRotation.Yaw = bRagdollFacingUpward ? PelvisRotation.Yaw - 180.0f : PelvisRotation.Yaw;

	GetOwner()->SetActorLocationAndRotation(NewActorLocation, NewActorRotation, false, nullptr, ETeleportType::TeleportPhysics);

	// Attach the mesh back and restore its default relative location.

	const auto& ActorTransform{GetOwner()->GetActorTransform()};

	FVector BaseTranslationOffset{FVector::Zero()};
	FQuat BaseRotationOffset;
	if (IsValid(CharacterOwner))
	{
		BaseTranslationOffset = CharacterOwner->GetBaseTranslationOffset();
		BaseRotationOffset = CharacterOwner->GetBaseRotationOffset();
	}
	MeshComponent->SetWorldLocationAndRotationNoPhysics(ActorTransform.TransformPositionNoScale(BaseTranslationOffset),
	                                                    ActorTransform.TransformRotation(BaseRotationOffset).Rotator());

	MeshComponent->AttachToComponent(RootPrimitive, FAttachmentTransformRules::KeepWorldTransform);

	if (MeshComponent->ShouldUseUpdateRateOptimizations())
	{
		// Disable URO for one frame to force the animation blueprint to update and get rid of the incorrect mesh pose.

		MeshComponent->bEnableUpdateRateOptimizations = false;

		GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]
		{
			MeshComponent->bEnableUpdateRateOptimizations = true;
		}));
	}

	// Restore the pelvis transform to the state it was in before we changed
	// the character and mesh transforms to keep its world transform unchanged.

	const auto& ReferenceSkeleton{MeshComponent->GetSkinnedAsset()->GetRefSkeleton()};

	const auto PelvisBoneIndex{ReferenceSkeleton.FindBoneIndex(PelvisBoneName)};
	if (PelvisBoneIndex >= 0)
	{
		// We expect the pelvis bone to be the root bone or attached to it, so we can safely use the mesh transform here.
		RagdollState.FinalRagdollPose.LocalTransforms[PelvisBoneIndex] = PelvisTransform.GetRelativeTransform(MeshComponent->GetComponentTransform());
	}

	bRagdolling = false;

	if (IsValid(CharacterOwner))
	{
		if (bGrounded)
		{
			CharacterOwner->GetCharacterMovement()->SetMovementMode(CharacterOwner->GetCharacterMovement()->DefaultLandMovementMode);
		}
		else
		{
			CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			CharacterOwner->GetCharacterMovement()->Velocity = RagdollState.Velocity;
		}
	}

	OnRagdollEnded(bGrounded);

	if (bGrounded && bPlayGetupMontageAfterRagdollEndedOnGround)
	{
		if (UAnimMontage* SelectedMontage = SelectGetUpMontage(bRagdollFacingUpward))
		{
			MeshComponent->GetAnimInstance()->Montage_Play(SelectedMontage);
		}
	}
}

void UGGS_RagdollComponent::OnRagdollEnded_Implementation(bool bGrounded)
{
	OnRagdollEndedEvent.Broadcast(bGrounded);
	// If the ragdoll is on the ground, set the movement mode to walking and play a get up montage. If not, set
	// the movement mode to falling and update the character movement velocity to match the last ragdoll velocity.

	// AlsCharacterMovement->SetMovementModeLocked(false);
	//

	//
	// SetLocomotionAction(FGameplayTag::EmptyTag);
	//
	// if (bGrounded && MeshComponent->GetAnimInstance()->Montage_Play(SelectGetUpMontage(bRagdollFacingUpward)) > 0.0f)
	// {
	// 	AlsCharacterMovement->SetInputBlocked(true);
	//
	// 	SetLocomotionAction(AlsLocomotionActionTags::GettingUp);
	// }
}

void UGGS_RagdollComponent::SetRagdollTargetLocation(const FVector& NewTargetLocation)
{
	if (RagdollTargetLocation != NewTargetLocation)
	{
		RagdollTargetLocation = NewTargetLocation;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, RagdollTargetLocation, this)

		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerSetRagdollTargetLocation(RagdollTargetLocation);
		}
	}
}

void UGGS_RagdollComponent::ServerSetRagdollTargetLocation_Implementation(const FVector_NetQuantize& NewTargetLocation)
{
	SetRagdollTargetLocation(NewTargetLocation);
}

void UGGS_RagdollComponent::RefreshRagdoll(float DeltaTime)
{
	if (!bRagdolling)
	{
		return;
	}

	// Since we are dealing with physics here, we should not use functions such as USkinnedMeshComponent::GetSocketTransform() as
	// they may return an incorrect result in situations like when the animation blueprint is not ticking or when URO is enabled.

	const auto* PelvisBody{MeshComponent->GetBodyInstance(PelvisBoneName)};
	FVector PelvisLocation;

	FPhysicsCommand::ExecuteRead(PelvisBody->ActorHandle, [this, &PelvisLocation](const FPhysicsActorHandle& ActorHandle)
	{
		PelvisLocation = FPhysicsInterface::GetTransform_AssumesLocked(ActorHandle, true).GetLocation();
		RagdollState.Velocity = FPhysicsInterface::GetLinearVelocity_AssumesLocked(ActorHandle);
	});

	const auto bLocallyControlled{PawnOwner->IsLocallyControlled() || (PawnOwner->GetLocalRole() >= ROLE_Authority && !IsValid(PawnOwner->GetController()))};

	if (bLocallyControlled)
	{
		SetRagdollTargetLocation(PelvisLocation);
	}

	// Prevent the capsule from going through the ground when the ragdoll is lying on the ground.

	// While we could get rid of the line trace here and just use RagdollTargetLocation
	// as the character's location, we don't do that because the camera depends on the
	// capsule's bottom location, so its removal will cause the camera to behave erratically.

	bool bGrounded;
	PawnOwner->SetActorLocation(RagdollTraceGround(bGrounded), false, nullptr, ETeleportType::TeleportPhysics);

	// Zero target location means that it hasn't been replicated yet, so we can't apply the logic below.

	if (!bLocallyControlled && !RagdollTargetLocation.IsZero())
	{
		// Apply ragdoll location corrections.

		static constexpr auto PullForce{750.0f};
		static constexpr auto InterpolationHalfLife{1.2f};

		RagdollState.PullForce = FMath::Lerp(RagdollState.PullForce, PullForce, DamperExactAlpha(DeltaTime, InterpolationHalfLife));

		const auto HorizontalSpeedSquared{RagdollState.Velocity.SizeSquared2D()};

		const auto PullForceBoneName{
			HorizontalSpeedSquared > FMath::Square(300.0f) ? SpineBoneName : PelvisBoneName
		};

		auto* PullForceBody{MeshComponent->GetBodyInstance(PullForceBoneName)};

		FPhysicsCommand::ExecuteWrite(PullForceBody->ActorHandle, [this](const FPhysicsActorHandle& ActorHandle)
		{
			if (!FPhysicsInterface::IsRigidBody(ActorHandle))
			{
				return;
			}

			const auto PullForceVector{
				RagdollTargetLocation - FPhysicsInterface::GetTransform_AssumesLocked(ActorHandle, true).GetLocation()
			};

			static constexpr auto MinPullForceDistance{5.0f};
			static constexpr auto MaxPullForceDistance{50.0f};

			if (PullForceVector.SizeSquared() > FMath::Square(MinPullForceDistance))
			{
				FPhysicsInterface::AddForce_AssumesLocked(
					ActorHandle, PullForceVector.GetClampedToMaxSize(MaxPullForceDistance) * RagdollState.PullForce, true, true);
			}
		});
	}

	// Use the speed to scale ragdoll joint strength for physical animation.

	static constexpr auto ReferenceSpeed{1000.0f};
	static constexpr auto Stiffness{25000.0f};

	const auto SpeedAmount{Clamp01(UE_REAL_TO_FLOAT(RagdollState.Velocity.Size() / ReferenceSpeed))};

	MeshComponent->SetAllMotorsAngularDriveParams(SpeedAmount * Stiffness, 0.0f, 0.0f);

	// Limit the speed of ragdoll bodies.

	if (RagdollState.SpeedLimitFrameTimeRemaining > 0)
	{
		RagdollState.SpeedLimitFrameTimeRemaining -= 1;

		ConstraintRagdollSpeed();
	}
}

FVector UGGS_RagdollComponent::RagdollTraceGround(bool& bGrounded) const
{
	auto RagdollLocation{!RagdollTargetLocation.IsZero() ? FVector{RagdollTargetLocation} : GetOwner()->GetActorLocation()};

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!IsValid(Character))
		return RagdollLocation;

	// We use a sphere sweep instead of a simple line trace to keep capsule
	// movement consistent between Ragdoll and regular character movement.

	const auto CapsuleRadius{Character->GetCapsuleComponent()->GetScaledCapsuleRadius()};
	const auto CapsuleHalfHeight{Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};

	const FVector TraceStart{RagdollLocation.X, RagdollLocation.Y, RagdollLocation.Z + 2.0f * CapsuleRadius};
	const FVector TraceEnd{RagdollLocation.X, RagdollLocation.Y, RagdollLocation.Z - CapsuleHalfHeight + CapsuleRadius};

	const auto CollisionChannel{Character->GetCharacterMovement()->UpdatedComponent->GetCollisionObjectType()};

	FCollisionQueryParams QueryParameters{TEXT("RagdollTraceGround"), false, GetOwner()};
	FCollisionResponseParams CollisionResponses;
	Character->GetCharacterMovement()->InitCollisionParams(QueryParameters, CollisionResponses);

	FHitResult Hit;
	bGrounded = GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity,
	                                             CollisionChannel, FCollisionShape::MakeSphere(CapsuleRadius),
	                                             QueryParameters, CollisionResponses);

	// #if ENABLE_DRAW_DEBUG
	// 	UAlsDebugUtility::DrawSweepSingleSphere(GetWorld(), TraceStart, TraceEnd, CapsuleRadius,
	// 	                                        bGrounded, Hit, {0.0f, 0.25f, 1.0f},
	// 	                                        {0.0f, 0.75f, 1.0f}, 0.0f);
	// #endif

	return {
		RagdollLocation.X, RagdollLocation.Y,
		bGrounded
			? Hit.Location.Z + CapsuleHalfHeight - CapsuleRadius + UCharacterMovementComponent::MIN_FLOOR_DIST
			: RagdollLocation.Z
	};
}

void UGGS_RagdollComponent::ConstraintRagdollSpeed() const
{
	MeshComponent->ForEachBodyBelow(NAME_None, true, false, [this](FBodyInstance* Body)
	{
		FPhysicsCommand::ExecuteWrite(Body->ActorHandle, [this](const FPhysicsActorHandle& ActorHandle)
		{
			if (!FPhysicsInterface::IsRigidBody(ActorHandle))
			{
				return;
			}

			auto Velocity{FPhysicsInterface::GetLinearVelocity_AssumesLocked(ActorHandle)};
			if (Velocity.SizeSquared() <= FMath::Square(RagdollState.SpeedLimit))
			{
				return;
			}

			Velocity.Normalize();
			Velocity *= RagdollState.SpeedLimit;

			FPhysicsInterface::SetLinearVelocity_AssumesLocked(ActorHandle, Velocity);
		});
	});
}

UAnimMontage* UGGS_RagdollComponent::SelectGetUpMontage_Implementation(bool bRagdollFacingUpward)
{
	if (GetUpBackMontage.IsNull() || GetUpFrontMontage.IsNull())
	{
		return nullptr;
	}
	return bRagdollFacingUpward ? GetUpBackMontage.LoadSynchronous() : GetUpFrontMontage.LoadSynchronous();
}


// Called when the game starts
void UGGS_RagdollComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = GetPawnChecked<APawn>();
	CharacterOwner = GetPawn<ACharacter>();

	if (CharacterOwner)
	{
		MeshComponent = CharacterOwner->GetMesh();
	}

	if (!MeshComponent)
	{
		MeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	}
	if (!MeshComponent)
	{
		GGS_CLOG(Warning, "Require skeletal mesh component for the Ragdoll to work.")
	}
}

float UGGS_RagdollComponent::DamperExactAlpha(float DeltaTime, float HalfLife)
{
	return 1.0f - FMath::InvExpApprox(0.6931471805599453f / (HalfLife + UE_SMALL_NUMBER) * DeltaTime);
}

float UGGS_RagdollComponent::Clamp01(float Value)
{
	return Value > 0.0f
		       ? Value < 1.0f
			         ? Value
			         : 1.0f
		       : 0.0f;
}


void UGGS_RagdollComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, RagdollTargetLocation, Parameters)
}

// Called every frame
void UGGS_RagdollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RefreshRagdoll(DeltaTime);
	// ...
}
