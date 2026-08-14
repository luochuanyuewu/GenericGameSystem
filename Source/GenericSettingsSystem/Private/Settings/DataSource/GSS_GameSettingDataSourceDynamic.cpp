// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/DataSource/GSS_GameSettingDataSourceDynamic.h"

#include "Engine/LocalPlayer.h"

//--------------------------------------
// FGSS_GameSettingDataSourceDynamic
//--------------------------------------

FGSS_GameSettingDataSourceDynamic::FGSS_GameSettingDataSourceDynamic(const TArray<FString>& InDynamicPath)
	: DynamicPath(InDynamicPath)
{
}

bool FGSS_GameSettingDataSourceDynamic::Resolve(ULocalPlayer* InLocalPlayer)
{
	return DynamicPath.Resolve(InLocalPlayer);
}

FString FGSS_GameSettingDataSourceDynamic::GetValueAsString(ULocalPlayer* InLocalPlayer) const
{
	FString OutStringValue;

	const bool bSuccess = PropertyPathHelpers::GetPropertyValueAsString(InLocalPlayer, DynamicPath, OutStringValue);
	ensure(bSuccess);

	return OutStringValue;
}

void FGSS_GameSettingDataSourceDynamic::SetValue(ULocalPlayer* InLocalPlayer, const FString& InStringValue)
{
	const bool bSuccess = PropertyPathHelpers::SetPropertyValueFromString(InLocalPlayer, DynamicPath, InStringValue);
	ensure(bSuccess);
}

FString FGSS_GameSettingDataSourceDynamic::ToString() const
{
	return DynamicPath.ToString();
}
