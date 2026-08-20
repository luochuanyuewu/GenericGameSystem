// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingEditCondition.h"
#include "GSS_WhenPlayingAsPrimaryPlayer.generated.h"

class ULocalPlayer;

/** Edit condition that exposes a setting only to the primary local player. / 仅向主本地玩家暴露设置的编辑条件。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_WhenPlayingAsPrimaryPlayer : public UGSS_GameSettingEditCondition
{
	GENERATED_BODY()

public:
	/** Creates a condition owned by Outer, normally the setting it will be added to. / 创建由 Outer 持有的条件，通常是即将添加该条件的设置。 */
	static UGSS_WhenPlayingAsPrimaryPlayer* Create(UObject* Outer);
	virtual void Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState) override;
};
