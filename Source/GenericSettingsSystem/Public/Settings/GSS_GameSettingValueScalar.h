// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValue.h"
#include "Math/Range.h"

#include "GSS_GameSettingValueScalar.generated.h"

class UObject;

UCLASS(abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueScalar : public UGSS_GameSettingValue
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueScalar();

	void SetValueNormalized(double NormalizedValue);
	double GetValueNormalized() const;

	TOptional<double> GetDefaultValueNormalized() const
	{
		TOptional<double> DefaultValue = GetDefaultValue();
		if (DefaultValue.IsSet())
		{
			return FMath::GetMappedRangeValueClamped(GetSourceRange(), TRange<double>(0, 1), DefaultValue.GetValue());
		}
		return TOptional<double>();
	}

	virtual TOptional<double> GetDefaultValue() const						PURE_VIRTUAL(, return TOptional<double>(););
	virtual void SetValue(double Value, EGSS_GameSettingChangeReason Reason = EGSS_GameSettingChangeReason::Change)	PURE_VIRTUAL(, );
	virtual double GetValue() const											PURE_VIRTUAL(, return 0;);
	virtual TRange<double> GetSourceRange() const							PURE_VIRTUAL(, return TRange<double>(););
	virtual double GetSourceStep() const									PURE_VIRTUAL(, return 0.01;);
	double GetNormalizedStepSize() const
	{
		TRange<double> SourceRange = GetSourceRange();
		return GetSourceStep() / FMath::Abs(SourceRange.GetUpperBoundValue() - SourceRange.GetLowerBoundValue());
	}
	virtual FText GetFormattedText() const									PURE_VIRTUAL(, return FText::GetEmpty(););
	
	virtual FString GetAnalyticsValue() const override
	{
		return LexToString(GetValue());
	}

protected:
};
