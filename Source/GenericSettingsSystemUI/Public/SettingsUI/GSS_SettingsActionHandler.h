// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "GSS_SettingsActionHandler.generated.h"

class UGSS_GameSetting;

/** Implement on a settings screen that handles named GSS action nodes. / 实现在处理命名 GSS Action 节点的设置界面上。 */
UINTERFACE(BlueprintType)
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsActionHandler : public UInterface
{
	GENERATED_BODY()
};

/** Receives an ActionId emitted by a settings action entry. / 接收设置 Action 条目发出的 ActionId。 */
class GENERICSETTINGSSYSTEMUI_API IGSS_SettingsActionHandler
{
	GENERATED_BODY()

public:
	/** Return true when the action was consumed by this screen. / 当前界面已消费该 Action 时返回 true。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GSS|Settings UI")
	bool ExecuteSettingsAction(FGameplayTag ActionId, UGSS_GameSetting* Setting);
};
