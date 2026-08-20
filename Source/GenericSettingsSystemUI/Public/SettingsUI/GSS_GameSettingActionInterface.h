// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "GSS_GameSettingActionInterface.generated.h"

class UGSS_GameSetting;

/**
 * Implemented by a pushed editor widget that receives a named settings action.
 * 由被推入的编辑器 Widget 实现，用于接收命名设置 Action。
 */
UINTERFACE(MinimalAPI, meta = (BlueprintType))
class UGSS_GameSettingActionInterface : public UInterface
{
	GENERATED_BODY()
};

class GENERICSETTINGSSYSTEMUI_API IGSS_GameSettingActionInterface
{
	GENERATED_BODY()

public:
	/** Hands the executing action and its setting to this editor widget. / 将正在执行的 Action 及其 Setting 交给此编辑器 Widget。 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "GSS|Settings UI")
	bool ExecuteActionForSetting(FGameplayTag ActionTag, UGSS_GameSetting* InSetting);
};
