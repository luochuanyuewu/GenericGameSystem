// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/EditCondition/GSS_WhenSettingHasValue.h"

#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_SettingValueAccessor.h"

void UGSS_WhenSettingHasValue::OnConditionInitialized_Implementation()
{
	if (!Setting || !OtherSettingId.IsValid())
	{
		return;
	}

	Setting->AddEditDependency(OtherSettingId);
}

void UGSS_WhenSettingHasValue::Evaluate_Implementation(FGSS_GameSettingEditableState& InOutEditState)
{
	if (!Setting || !OtherSettingId.IsValid())
	{
		return;
	}

	UGSS_GameSettingRegistry* Registry = Setting->GetRegistry();
	const UGSS_GameSettingValueDiscreteDynamic* OtherSetting = Registry
		? Cast<UGSS_GameSettingValueDiscreteDynamic>(Registry->FindSettingById(OtherSettingId))
		: nullptr;
	if (!OtherSetting)
	{
		return;
	}

	const FString CurrentValue = OtherSetting->GetValueAsString();
	bool bMatches = false;
	for (const FString& Candidate : MatchingValues)
	{
		if (FGSS_SettingValueAccessor::AreSerializedValuesEqual(CurrentValue, Candidate))
		{
			bMatches = true;
			break;
		}
	}

	if (bInvertMatch)
	{
		bMatches = !bMatches;
	}
	if (!bMatches)
	{
		return;
	}

	if (bHideWhenMatched)
	{
		InOutEditState.Kill(TEXT("Sibling setting value hides this setting."));
	}
	else
	{
		InOutEditState.Disable(DisabledReason);
	}
}

UGSS_WhenSettingHasValue* UGSS_WhenSettingHasValue::DisableIfHasAnyValue(UObject* Outer, FGameplayTag InOtherSettingId, TArray<FString> Values, const FText& Reason)
{
	UGSS_WhenSettingHasValue* Condition = NewObject<UGSS_WhenSettingHasValue>(Outer);
	Condition->OtherSettingId = InOtherSettingId;
	Condition->MatchingValues = MoveTemp(Values);
	Condition->bInvertMatch = false;
	Condition->DisabledReason = Reason;
	return Condition;
}

UGSS_WhenSettingHasValue* UGSS_WhenSettingHasValue::DisableIfLacksValue(UObject* Outer, FGameplayTag InOtherSettingId, TArray<FString> Values, const FText& Reason)
{
	UGSS_WhenSettingHasValue* Condition = NewObject<UGSS_WhenSettingHasValue>(Outer);
	Condition->OtherSettingId = InOtherSettingId;
	Condition->MatchingValues = MoveTemp(Values);
	Condition->bInvertMatch = true;
	Condition->DisabledReason = Reason;
	return Condition;
}
