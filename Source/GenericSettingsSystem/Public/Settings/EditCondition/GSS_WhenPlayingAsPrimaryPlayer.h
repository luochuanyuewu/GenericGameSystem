// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Settings/GSS_GameSettingFilterState.h"

class ULocalPlayer;


class GENERICSETTINGSSYSTEM_API FGSS_WhenPlayingAsPrimaryPlayer : public FGSS_GameSettingEditCondition
{
public:
	static TSharedRef<FGSS_WhenPlayingAsPrimaryPlayer> Get();

	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState) const override;
};
