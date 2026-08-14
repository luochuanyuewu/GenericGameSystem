// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingFilterState.h"

class FGSS_WhenCondition : public FGSS_GameSettingEditCondition
{
public:
	FGSS_WhenCondition(TFunction<void(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState&)>&& InInlineEditCondition)
		: InlineEditCondition(InInlineEditCondition)
	{
	}

	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState) const override
	{
		InlineEditCondition(InLocalPlayer, InOutEditState);
	}

	virtual FString ToString() const override
	{
		return TEXT("Inline Edit Condition");
	}

private:
	TFunction<void(const ULocalPlayer* InLocalPlayer, FGSS_GameSettingEditableState& InOutEditState)> InlineEditCondition;
};
