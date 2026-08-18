// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#pragma once

#include "GameFramework/SaveGame.h"
#include "GSS_SettingsShared.generated.h"

/**
 * Per-local-player, portable settings saved through UE's LocalPlayerSaveGame API.
 * 每个本地玩家可携带的设置，通过 UE 的 LocalPlayerSaveGame API 保存。
 *
 * Derive from this class in a project to add SaveGame properties and UFUNCTION getters/setters for Shared Accessors,
 * then override ApplySettings to apply committed values to project systems.
 * 项目可继承此类，添加 SaveGame 属性及供 Shared Accessor 调用的 UFUNCTION Getter/Setter，
 * 并重写 ApplySettings，将已提交值应用到项目系统。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEM_API UGSS_SettingsShared : public ULocalPlayerSaveGame
{
	GENERATED_BODY()

public:
	/** Applies already committed preferences to game systems before the object is saved. / 在对象保存前将已提交偏好应用到游戏系统。 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings")
	void ApplySettings();
};
