// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once


#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

#include "GCMS_CameraSystemComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCanvas;
class AHUD;
class UGCMS_CameraMode;
class UGCMS_CameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass>
class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UGCMS_CameraMode>, FGMSCameraModeDelegate);


/**
 * Runtime entry component for camera-mode-stack driven view updates.
 * 基于相机模式栈驱动视角更新的运行时入口组件。
 * @details Intended to live on the view target actor and push one camera mode decision each tick.
 * @细节 设计上挂载在视角目标 Actor 上，并在每帧推入一个相机模式决策。
 */
UCLASS(ClassGroup=GCMS, meta=(BlueprintSpawnableComponent))
class GENERICCAMERASYSTEM_API UGCMS_CameraSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGCMS_CameraSystemComponent(const FObjectInitializer& ObjectInitializer);

	static void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GCMS|Camera", meta=(DefaultToSelf="Actor"))
	static UGCMS_CameraSystemComponent* GetCameraSystemComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGCMS_CameraSystemComponent>() : nullptr); }

	/**
	 * Returns the target actor that the camera is looking at.
	 */
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Delegate used to query for the best camera mode.
	// 每帧用于查询“当前最优模式”的委托，应保持幂等且避免副作用。
	FGMSCameraModeDelegate DetermineCameraModeDelegate;

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	// 为 FOV 追加单帧偏移；该偏移用于临时镜头冲击感，不应作为持久配置。
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	// Push specified Camera Mode.
	UFUNCTION(BlueprintCallable, Category="GCMS|Camera")
	void PushCameraMode(TSubclassOf<UGCMS_CameraMode> NewCameraMode);

	UFUNCTION(BlueprintCallable, Category="GCMS|Camera")
	void PushDefaultCameraMode();

	/**
	 * Binds concrete camera/spring-arm pair that will receive blended mode outputs.
	 * 绑定接收模式混合输出的相机与弹簧臂组件。
	 */
	UFUNCTION(BlueprintCallable, Category="GCMS|Camera")
	void Initialize(UCameraComponent* NewCameraComponent, USpringArmComponent* NewSpringArmComponent);

	virtual void DrawDebug(UCanvas* Canvas) const;

	/**
	 * Gets the camera mode tag associated with the top layer and the blend weight of it
	 * 返回顶层相机模式的tag和当前权重。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GCMS|Camera")
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GCMS|Camera")
	UCameraComponent* GetAssociatedCamera() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GCMS|Camera")
	USpringArmComponent* GetAssociatedSprintArm() const;

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

protected:
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateCameraModes();

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> AssociatedCameraComponent;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> AssociatedSprintArmComponent;

	// Stack used to blend the camera modes.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="GCMS|Camera", meta=(ShowInnerProperties))
	TObjectPtr<UGCMS_CameraModeStack> CameraModeStack;

	// Default camera mode will used.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GCMS|Camera")
	TSubclassOf<UGCMS_CameraMode> DefaultCameraMode;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
};
