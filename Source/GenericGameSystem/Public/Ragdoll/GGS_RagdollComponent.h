// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GGS_RagdollStructLibrary.h"
#include "Components/PawnComponent.h"

#include "GGS_RagdollComponent.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGGS_RagdollStartedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGGS_RagdollEndedSignature, bool, bGrounded);

/**
 * Network-aware ragdoll controller with explicit authority transition and remote pose correction.
 * 具备明确权威切换与远端姿态纠正的网络化布娃娃控制组件。
 */
UCLASS(ClassGroup=(GGS), Blueprintable, meta=(BlueprintSpawnableComponent))
class GENERICGAMESYSTEM_API UGGS_RagdollComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGGS_RagdollComponent(const FObjectInitializer& ObjectInitializer);


	const FGGS_RagdollState& GetRagdollState() const;

	bool IsRagdollAllowedToStart() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GGS|Ragdoll")
	void SetMeshComponent(USkeletalMeshComponent* InMeshComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GGS|Ragdoll")
	bool IsRagdolling() const;

	/**
	 * Entry point for ragdoll start request; authority decides final multicast execution.
	 * 布娃娃开始请求入口；最终是否多播执行由权威端裁决。
	 */
	UFUNCTION(BlueprintCallable, Category = "GGS|Ragdoll")
	void StartRagdoll();

	UFUNCTION(BlueprintCallable, Category = "GGS|Ragdoll")
	virtual void LocalStartRagdoll();

	UPROPERTY(BlueprintAssignable, Category="Event")
	FGGS_RagdollStartedSignature OnRagdollStartedEvent;
	UPROPERTY(BlueprintAssignable, Category="Event")
	FGGS_RagdollEndedSignature OnRagdollEndedEvent;

private:
	UFUNCTION(Server, Reliable)
	void ServerStartRagdoll();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartRagdoll();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "GGS|Ragdoll")
	void OnRagdollStarted();

public:
	bool IsRagdollAllowedToStop() const;

	/**
	 * Entry point for ragdoll stop request; returns false when role/state does not allow transition.
	 * 布娃娃停止请求入口；当角色权限或状态不允许切换时返回 false。
	 */
	UFUNCTION(BlueprintCallable, Category = "GGS|Ragdoll", Meta = (ReturnDisplayName = "Success"))
	bool StopRagdoll();

	UFUNCTION(BlueprintCallable, Category = "GGS|Ragdoll")
	virtual void LocalStopRagdoll();

private:
	UFUNCTION(Server, Reliable)
	void ServerStopRagdoll();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopRagdoll();

protected:
	/**
	 * Chooses montage according to final grounded orientation after physics settles.
	 * 根据物理稳定后的最终朝向选择起身蒙太奇。
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "GGS|Ragdoll")
	UAnimMontage* SelectGetUpMontage(bool bRagdollFacingUpward);

	UFUNCTION(BlueprintNativeEvent, Category = "GGS|Ragdoll")
	void OnRagdollEnded(bool bGrounded);
	virtual void OnRagdollEnded_Implementation(bool bGrounded);

private:
	void SetRagdollTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Unreliable)
	void ServerSetRagdollTargetLocation(const FVector_NetQuantize& NewTargetLocation);

	void RefreshRagdoll(float DeltaTime);

	FVector RagdollTraceGround(bool& bGrounded) const;

	void ConstraintRagdollSpeed() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float DamperExactAlpha(float DeltaTime, float HalfLife);

	float Clamp01(float Value);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll Settings")
	FName PelvisBoneName{TEXT("pelvis")};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ragdoll Settings")
	FName SpineBoneName{TEXT("spine_03")};

	// If checked, the ragdoll's speed will be limited by the character's last speed for a few frames
	// after activation. This hack is used to prevent the ragdoll from getting a very high initial speed
	// at unstable FPS, which can be reproduced by jumping and activating the ragdoll at the same time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll Settings")
	uint8 bLimitInitialRagdollSpeed : 1 {true};

	/**
	 * If enabled, play get-up montage only when stop result is grounded to avoid airborne pose conflicts.
	 * 启用后仅在落地停止时播放起身蒙太奇，避免与空中状态姿态冲突。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll Settings")
	bool bPlayGetupMontageAfterRagdollEndedOnGround{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll Settings", meta=(EditCondition="bPlayGetupMontageAfterRagdollEndedOnGround"))
	TSoftObjectPtr<UAnimMontage> GetUpFrontMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ragdoll Settings", meta=(EditCondition="bPlayGetupMontageAfterRagdollEndedOnGround"))
	TSoftObjectPtr<UAnimMontage> GetUpBackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragdoll State", Transient)
	bool bRagdolling{false};

	/**
	 * Authoritative pelvis target used for remote correction; quantized for bandwidth stability.
	 * 用于远端纠正的权威骨盆目标点；采用量化以稳定带宽成本。
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragdoll State", Transient, Replicated)
	FVector_NetQuantize RagdollTargetLocation{ForceInit};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragdoll State", Transient)
	FGGS_RagdollState RagdollState;

	UPROPERTY(VisibleAnywhere, Category = "Ragdoll State", Transient)
	TObjectPtr<USkeletalMeshComponent> MeshComponent{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Ragdoll State", Transient)
	TObjectPtr<APawn> PawnOwner{nullptr};

	UPROPERTY(VisibleAnywhere, Category = "Ragdoll State", Transient)
	TObjectPtr<ACharacter> CharacterOwner{nullptr};

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
