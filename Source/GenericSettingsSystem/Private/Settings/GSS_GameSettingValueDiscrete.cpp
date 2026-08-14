// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingValueDiscrete
//--------------------------------------

UGSS_GameSettingValueDiscrete::UGSS_GameSettingValueDiscrete()
{

}

FString UGSS_GameSettingValueDiscrete::GetAnalyticsValue() const
{
	const TArray<FText> Options = GetDiscreteOptions();
	const int32 CurrentOptionIndex = GetDiscreteOptionIndex();
	if (Options.IsValidIndex(CurrentOptionIndex))
	{
		const FString* SourceString = FTextInspector::GetSourceString(Options[CurrentOptionIndex]);
		if (SourceString)
		{
			return *SourceString;
		}
	}

	return TEXT("<Unknown Index>");
}

#undef LOCTEXT_NAMESPACE

