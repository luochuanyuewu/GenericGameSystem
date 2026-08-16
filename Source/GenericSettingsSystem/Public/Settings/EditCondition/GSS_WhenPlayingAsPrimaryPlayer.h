// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_SettingEditCondition.h"
#include "GSS_WhenPlayingAsPrimaryPlayer.generated.h"

class ULocalPlayer;

/** Edit condition that exposes a setting only to the primary local player. / 仅向主本地玩家暴露设置的编辑条件。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_WhenPlayingAsPrimaryPlayer : public UGSS_SettingEditCondition
{
	GENERATED_BODY()

public:
	virtual void Evaluate_Implementation(UGSS_SettingEditableState* InOutEditState) override;
};
