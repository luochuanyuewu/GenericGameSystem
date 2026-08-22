// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueScalarDynamic.h"

#include "Settings/GSS_GameSettingsDefinitions.h"
#include "UObject/WeakObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueScalarDynamic)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

//////////////////////////////////////////////////////////////////////////
// SettingScalarFormats
//////////////////////////////////////////////////////////////////////////

static FText PercentFormat = LOCTEXT("PercentFormat", "{0}%");

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::Raw([](double SourceValue, double NormalizedValue) {
	return FText::AsNumber(SourceValue);
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::RawOneDecimal([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 1;
	FormattingOptions.MaximumFractionalDigits = 1;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::RawTwoDecimals([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 2;
	FormattingOptions.MaximumFractionalDigits = 2;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::SourceAsInteger([](double SourceValue, double NormalizedValue) {
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumIntegralDigits = 1;
	FormattingOptions.MinimumFractionalDigits = 0;
	FormattingOptions.MaximumFractionalDigits = 0;
	return FText::AsNumber(SourceValue, &FormattingOptions);
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::ZeroToOnePercent([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(100.0 * NormalizedValue));
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::ZeroToOnePercent_OneDecimal([](double SourceValue, double NormalizedValue) {
	const FNumberFormattingOptions& FormattingOptions = GetOneDecimalFormattingOptions();
	const double NormalizedValueTo100_0 = FMath::RoundHalfFromZero(1000.0 * NormalizedValue);
	return FText::Format(PercentFormat, FText::AsNumber(NormalizedValueTo100_0 / 10.0, &FormattingOptions));
});

const FNumberFormattingOptions& UGSS_GameSettingValueScalarDynamic::GetOneDecimalFormattingOptions()
{
	static FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumFractionalDigits = 1;
	FormattingOptions.MaximumFractionalDigits = 1;

	return FormattingOptions;
}

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::SourceAsPercent1([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(100.0 * SourceValue));
});

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::SourceAsPercent100([](double SourceValue, double NormalizedValue) {
	return FText::Format(PercentFormat, (int32)FMath::RoundHalfFromZero(SourceValue));
});

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueScalarDynamic
//////////////////////////////////////////////////////////////////////////

UGSS_GameSettingValueScalarDynamic::UGSS_GameSettingValueScalarDynamic()
{
}

void UGSS_GameSettingValueScalarDynamic::OnInitialized()
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(DisplayFormat, TEXT("%s: Has no DisplayFormat set.  Please call SetDisplayFormat."), *GetDevName().ToString());
#endif

	Super::OnInitialized();
}

void UGSS_GameSettingValueScalarDynamic::StoreInitial()
{
	FString StoredValue;
	Accessor.GetValue(LocalPlayer, StoredValue);
	InitialValue = StoredValue.IsEmpty() ? DefaultValue.Get(0.0) : FCString::Atod(*StoredValue);
	PendingValue = InitialValue;
}

void UGSS_GameSettingValueScalarDynamic::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValue(DefaultValue.GetValue(), EGSS_GameSettingChangeReason::ResetToDefault);
	}
}

void UGSS_GameSettingValueScalarDynamic::RestoreToInitial()
{
	SetValue(InitialValue, EGSS_GameSettingChangeReason::RestoreToInitial);
}

void UGSS_GameSettingValueScalarDynamic::SetAccessor(const FGSS_SettingValueAccessor& InAccessor)
{
	Accessor = InAccessor;
}

void UGSS_GameSettingValueScalarDynamic::SetDefaultValue(double InValue)
{
	DefaultValue = InValue;
}

void UGSS_GameSettingValueScalarDynamic::SetDisplayFormat(FSettingScalarFormatFunction InDisplayFormat)
{
	DisplayFormat = InDisplayFormat;
}

FSettingScalarFormatFunction UGSS_GameSettingValueScalarDynamic::GetBuiltInDisplayFormat(EGSS_SettingScalarDisplayFormat Format)
{
	switch (Format)
	{
	case EGSS_SettingScalarDisplayFormat::RawOneDecimal:
		return RawOneDecimal;
	case EGSS_SettingScalarDisplayFormat::RawTwoDecimals:
		return RawTwoDecimals;
	case EGSS_SettingScalarDisplayFormat::ZeroToOnePercent:
		return ZeroToOnePercent;
	case EGSS_SettingScalarDisplayFormat::ZeroToOnePercentOneDecimal:
		return ZeroToOnePercent_OneDecimal;
	case EGSS_SettingScalarDisplayFormat::SourceAsPercent1:
		return SourceAsPercent1;
	case EGSS_SettingScalarDisplayFormat::SourceAsPercent100:
		return SourceAsPercent100;
	case EGSS_SettingScalarDisplayFormat::SourceAsInteger:
		return SourceAsInteger;
	default:
		return Raw;
	}
}

void UGSS_GameSettingValueScalarDynamic::SetSourceRangeAndStep(const TRange<double>& InRange, double InStep)
{
	SourceRange = InRange;
	SourceStep = InStep;
}

void UGSS_GameSettingValueScalarDynamic::SetMinimumLimit(const TOptional<double>& InMinimum)
{
	Minimum = InMinimum;
}

void UGSS_GameSettingValueScalarDynamic::SetMaximumLimit(const TOptional<double>& InMaximum)
{
	Maximum = InMaximum;
}

double UGSS_GameSettingValueScalarDynamic::GetValue() const
{
	return PendingValue;
}

TRange<double> UGSS_GameSettingValueScalarDynamic::GetSourceRange() const
{
	return SourceRange;
}

double UGSS_GameSettingValueScalarDynamic::GetSourceStep() const
{
	return SourceStep;
}

TOptional<double> UGSS_GameSettingValueScalarDynamic::GetDefaultValue() const
{
	return DefaultValue;
}

void UGSS_GameSettingValueScalarDynamic::SetValue(double InValue, EGSS_GameSettingChangeReason Reason)
{
	InValue = FMath::RoundHalfFromZero(InValue / SourceStep);
	InValue = InValue * SourceStep;

	if (Minimum.IsSet())
	{
		InValue = FMath::Max(Minimum.GetValue(), InValue);
	}

	if (Maximum.IsSet())
	{
		InValue = FMath::Min(Maximum.GetValue(), InValue);
	}

	PendingValue = InValue;
	NotifySettingChanged(Reason);
}

void UGSS_GameSettingValueScalarDynamic::OnApply()
{
	Accessor.SetValue(LocalPlayer, LexToString(PendingValue));
}

FText UGSS_GameSettingValueScalarDynamic::GetFormattedText() const
{
	const double SourceValue = GetValue();
	const double NormalizedValue = GetValueNormalized();

	return DisplayFormat(SourceValue, NormalizedValue);
}

#undef LOCTEXT_NAMESPACE
