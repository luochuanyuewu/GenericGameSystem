// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingsProvider.h"

#include "GSS_LanguageSettingsProvider.generated.h"

/**
 * Registers an optional language picker under GSS.Settings.Language.
 * 在 GSS.Settings.Language 下注册可选的语言选择器。
 *
 * Add this class to Startup Provider Classes when the project ships more than one culture.
 * The picker enumerates cooked game cultures at runtime; it does not depend on Shared Settings.
 * 项目提供多种 Culture 时，将此类加入 Startup Provider Classes。
 * 选择器在运行时枚举已 cook 的游戏 Culture，不依赖 Shared Settings。
 */
UCLASS(BlueprintType, Blueprintable)
class GENERICSETTINGSSYSTEMCOMMON_API UGSS_LanguageSettingsProvider : public UGSS_GameSettingsProvider
{
	GENERATED_BODY()

public:
	virtual void RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder) override;
};
