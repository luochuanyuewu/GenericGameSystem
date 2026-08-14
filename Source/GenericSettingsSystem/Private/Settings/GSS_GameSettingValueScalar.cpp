// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueScalar.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueScalar)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingValueScalar
//--------------------------------------

UGSS_GameSettingValueScalar::UGSS_GameSettingValueScalar()
{

}

void UGSS_GameSettingValueScalar::SetValueNormalized(double NormalizedValue)
{
	SetValue(FMath::GetMappedRangeValueClamped(TRange<double>(0, 1), GetSourceRange(), NormalizedValue));
}

double UGSS_GameSettingValueScalar::GetValueNormalized() const
{
	return FMath::GetMappedRangeValueClamped(GetSourceRange(), TRange<double>(0, 1), GetValue());
}

#undef LOCTEXT_NAMESPACE

