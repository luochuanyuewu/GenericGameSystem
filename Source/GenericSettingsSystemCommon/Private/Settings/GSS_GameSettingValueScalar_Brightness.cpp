// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueScalar_Brightness.h"

#include "Engine/Engine.h"
#include "Settings/GSS_GameSettingValue.h"
#include "Misc/ConfigCacheIni.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueScalar_Brightness)

namespace GSS_Brightness
{
	static const TCHAR* ConfigSection = TEXT("GSS.Video");
	static const TCHAR* ConfigKey = TEXT("DisplayGamma");
	static constexpr float DefaultGamma = 2.2f;
}

UGSS_GameSettingValueScalar_Brightness::UGSS_GameSettingValueScalar_Brightness()
{
	SetDefaultValue(GSS_Brightness::DefaultGamma);
	SetSourceRangeAndStep(TRange<double>(1.7, 2.7), 0.01);
	SetDisplayFormat([](double /*SourceValue*/, double NormalizedValue)
	{
		return FText::Format(NSLOCTEXT("GSS_GameSettingValueScalar_Brightness", "BrightnessFormat", "{0}%"),
			static_cast<int32>(FMath::GetMappedRangeValueClamped(FVector2D(0.0, 1.0), FVector2D(50.0, 150.0), NormalizedValue)));
	});
}

void UGSS_GameSettingValueScalar_Brightness::OnInitialized()
{
	float SavedGamma = GSS_Brightness::DefaultGamma;
	if (TryLoadSavedGamma(SavedGamma))
	{
		ApplyGamma(SavedGamma);
	}

	UGSS_GameSettingValue::OnInitialized();
}

void UGSS_GameSettingValueScalar_Brightness::StoreInitial()
{
	InitialValue = GetValue();
}

double UGSS_GameSettingValueScalar_Brightness::GetValue() const
{
	return GEngine ? GEngine->DisplayGamma : DefaultValue.Get(GSS_Brightness::DefaultGamma);
}

void UGSS_GameSettingValueScalar_Brightness::SetValue(double InValue, EGSS_GameSettingChangeReason Reason)
{
	InValue = SanitizeSourceValue(InValue);
	ApplyGamma(static_cast<float>(InValue));
	NotifySettingChanged(Reason);
}

void UGSS_GameSettingValueScalar_Brightness::OnApply()
{
	SaveGamma(static_cast<float>(GetValue()));
}

bool UGSS_GameSettingValueScalar_Brightness::TryLoadSavedGamma(float& OutGamma)
{
	return GConfig && GConfig->GetFloat(GSS_Brightness::ConfigSection, GSS_Brightness::ConfigKey, OutGamma, GGameUserSettingsIni);
}

void UGSS_GameSettingValueScalar_Brightness::SaveGamma(float Gamma)
{
	if (!GConfig)
	{
		return;
	}

	GConfig->SetFloat(GSS_Brightness::ConfigSection, GSS_Brightness::ConfigKey, Gamma, GGameUserSettingsIni);
	GConfig->Flush(false, GGameUserSettingsIni);
}

void UGSS_GameSettingValueScalar_Brightness::ApplyGamma(float Gamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = Gamma;
	}
}
