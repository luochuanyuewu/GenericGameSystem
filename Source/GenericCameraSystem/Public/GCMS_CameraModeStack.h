// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GCMS_CameraMode.h"
#include "UObject/Object.h"
#include "GCMS_CameraModeStack.generated.h"


/**
 * Blend stack for camera modes where each entry contributes by weight over lower layers.
 * 相机模式混合栈，每层通过权重覆盖其下层结果。
 * @details Modes are cached per class and reused; stack mutation only changes active ordering/weights.
 * @细节 模式按类缓存并复用；栈操作只改变活跃顺序和权重，不重复构造实例。
 */
UCLASS()
class GENERICCAMERASYSTEM_API UGCMS_CameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UGCMS_CameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UGCMS_CameraMode> CameraModeClass);

	void PopCameraMode(TSubclassOf<UGCMS_CameraMode> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FGCMS_CameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	// 获取顶层模式标签与其当前混合权重，用于玩法侧判断相机语义状态。
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	/**
	 * Get or create new camera mode. 
	 */
	UGCMS_CameraMode* GetCameraModeInstance(TSubclassOf<UGCMS_CameraMode> CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FGCMS_CameraModeView& OutCameraModeView) const;

protected:
	bool bIsActive;

	/**
	 * Cached mode instances by class to avoid per-frame allocation churn.
	 * 按类缓存的模式实例池，避免每帧分配抖动。
	 */
	UPROPERTY()
	TArray<TObjectPtr<UGCMS_CameraMode>> CameraModeInstances;

	/**
	 * Active stack entries; order encodes blend precedence.
	 * 当前活跃模式栈；顺序即混合优先级。
	 */
	UPROPERTY()
	TArray<TObjectPtr<UGCMS_CameraMode>> CameraModeStack;
};
