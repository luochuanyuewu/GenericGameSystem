// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"

#include "GSS_GameSettingValueDiscrete_Language.generated.h"

/**
 * Runtime language picker enumerating cultures cooked with the game.
 * 运行时语言选择器，枚举随游戏一起 cook 的 Culture。
 *
 * The empty option is System Default. Apply writes Unreal's Internationalization Culture key in
 * GameUserSettings.ini; some localization changes still require a full restart.
 * 空选项表示系统默认。Apply 会写入 GameUserSettings.ini 中的 Internationalization Culture；
 * 部分本地化变更仍需要完全重启。
 */
UCLASS()
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_GameSettingValueDiscrete_Language : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	/** Rebuilds options from localized game cultures. / 根据游戏已本地化的 Culture 重建选项。 */
	void RefreshCultureOptions();
	virtual void StoreInitial() override;
	virtual int32 GetDiscreteOptionIndex() const override;

protected:
	virtual void OnInitialized() override;
	virtual bool OnApply() override;

private:
	static bool IsUsingDefaultCulture();
	static FString GetCurrentCultureName();
	static FText BuildCultureDisplayName(const FString& CultureName);
	int32 FindBestOptionIndex(const FString& CultureName) const;
};
