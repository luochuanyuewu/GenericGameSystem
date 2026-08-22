// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "UObject/ObjectKey.h"
#include "UObject/WeakObjectPtrTemplates.h"

enum class EGSS_GameSettingChangeReason : uint8;

class UGSS_GameSetting;
class UGSS_GameSettingRegistry;
struct FObjectKey;

/**
 * Tracks pending edits for a Registry and coordinates transactional Apply/Cancel behavior.
 * 跟踪 Registry 的待应用修改，并协调事务式 Apply/Cancel 行为。
 *
 * The subsystem owns one tracker per LocalPlayer.
 * 子系统为每个 LocalPlayer 持有一个 Tracker。
 */
class GENERICSETTINGSSYSTEM_API FGSS_GameSettingRegistryChangeTracker : public FNoncopyable
{
public:
	FGSS_GameSettingRegistryChangeTracker();
	~FGSS_GameSettingRegistryChangeTracker();

	/** Starts replacing any previous Registry subscription. / 开始监听指定 Registry，并替换此前订阅。 */
	void WatchRegistry(UGSS_GameSettingRegistry* InRegistry);
	/** Removes the current Registry subscription. / 解除当前 Registry 订阅。 */
	void StopWatchingRegistry();

	/** Applies every dirty value, stores the new initial, then clears dirty state. / 应用全部脏值、记录新的初始值，然后清除脏状态。 */
	void ApplyChanges();

	/** Restores all dirty values to their last stored initial value and clears dirty state. / 将全部脏值恢复为最后记录的初始值并清除脏状态。 */
	void RestoreToInitial();

	/** Removes a setting that is no longer part of the registry from the pending apply set. / 从待应用集合中移除已不属于 Registry 的设置。 */
	void DiscardSetting(UGSS_GameSetting* Setting);

	/** Clears every pending change without writing or restoring values. / 不写入也不恢复值，直接清除全部待应用状态。 */
	void ClearDirtyState();

	bool IsRestoringSettings() const { return bRestoringSettings; }
	bool HaveSettingsBeenChanged() const { return bSettingsChanged; }

private:
	void HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);

	bool bSettingsChanged = false;
	bool bRestoringSettings = false;

	TWeakObjectPtr<UGSS_GameSettingRegistry> Registry;
	TMap<FObjectKey, TWeakObjectPtr<UGSS_GameSetting>> DirtySettings;
};
