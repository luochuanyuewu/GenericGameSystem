// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValue.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValue)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//--------------------------------------
// UGSS_GameSettingValue
//--------------------------------------

UGSS_GameSettingValue::UGSS_GameSettingValue()
{
}

void UGSS_GameSettingValue::OnInitialized()
{
	Super::OnInitialized();

#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!DescriptionRichText.IsEmpty() || DynamicDetails.IsBound(), TEXT("You must provide a description or it must specify a dynamic details function for settings with values."));
#endif

	StoreInitial();
}

#undef LOCTEXT_NAMESPACE
