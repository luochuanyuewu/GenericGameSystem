// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscrete.h"

#include "GSS_GameSettingValueDiscrete_Language.generated.h"

/**
 * Runtime language picker enumerating cultures cooked with the game.
 * 运行时语言选择器，枚举随游戏一起 cook 的 Culture。
 *
 * This is a custom Discrete node, not a Dynamic setting. The empty option is System Default.
 * Culture changes stay staged until Apply writes Unreal's Internationalization Culture key in
 * GameUserSettings.ini; some localization changes still require a full restart.
 * 这是自定义 Discrete 节点，不是 Dynamic 设置。空选项表示系统默认。
 * Culture 变更会暂存，直到 Apply 写入 GameUserSettings.ini 中的 Internationalization Culture；
 * 部分本地化变更仍需要完全重启。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Language : public UGSS_GameSettingValueDiscrete
{
	GENERATED_BODY()

public:
	/** Rebuilds options from localized game cultures. / 根据游戏已本地化的 Culture 重建选项。 */
	void RefreshCultureOptions();

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual int32 GetDiscreteOptionDefaultIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	virtual void OnInitialized() override;
	virtual void OnApply() override;

private:
	static bool IsUsingDefaultCulture();
	static FString GetCurrentCultureName();
	static FText BuildCultureDisplayName(const FString& CultureName);
	int32 FindBestOptionIndex(const FString& CultureName) const;

	TArray<FString> AvailableCultureNames;
	FString InitialValue;
	FString PendingCulture;
};
