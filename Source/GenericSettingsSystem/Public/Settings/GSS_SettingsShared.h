// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#pragma once

#include "GameFramework/SaveGame.h"
#include "GSS_SettingsShared.generated.h"

/**
 * Per-local-player, portable settings saved through UE's LocalPlayerSaveGame API.
 * 每个本地玩家可携带的设置，通过 UE 的 LocalPlayerSaveGame API 保存。
 *
 * Derive from this class in a project to add SaveGame properties and UFUNCTION getters/setters for Shared Accessors.
 * Dynamic settings invoke those setters immediately so values such as volume can preview live.
 * Override ApplySettings to apply stored preferences to project systems: it runs after load and on ApplyChanges.
 * Override SaveSettings to add extra persistence after the default async LocalPlayer SaveGame write.
 * 项目可继承此类，添加 SaveGame 属性及供 Shared Accessor 调用的 UFUNCTION Getter/Setter。
 * Dynamic 设置会立即调用这些 Setter，因此音量等值可以即时预览。
 * 重写 ApplySettings 将已存偏好应用到项目系统：加载后与 ApplyChanges 时都会调用。
 * 重写 SaveSettings 可在默认的异步 LocalPlayer SaveGame 写入之后追加额外持久化。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEM_API UGSS_SettingsShared : public ULocalPlayerSaveGame
{
	GENERATED_BODY()

public:
	/**
	 * Applies the current preferences to game systems.
	 * Called after LoadOrCreate on startup and Reload, and again from ApplyChanges.
	 * Deserialization writes UPROPERTY values without setters, so project overrides must apply those stored values here.
	 * 将当前偏好应用到游戏系统。
	 * 启动与 Reload 时在 LoadOrCreate 之后调用，ApplyChanges 时再次调用。
	 * 反序列化会直接写入 UPROPERTY 而不走 Setter，因此项目重写必须在这里应用已存值。
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings")
	void ApplySettings();

	/** Persists this object. The default implementation saves asynchronously for the owning LocalPlayer. / 持久化此对象。默认实现为所属 LocalPlayer 异步保存。 */
	UFUNCTION(BlueprintNativeEvent, Category = "GSS|Settings")
	void SaveSettings();
};
