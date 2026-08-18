// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValue.h"
#include "Math/Range.h"

#include "GSS_GameSettingValueScalar.generated.h"

class UObject;

/** Abstract continuous numeric value with normalized [0,1] helpers. / 带有归一化 [0,1] 辅助方法的抽象连续数值。 */
UCLASS(abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueScalar : public UGSS_GameSettingValue
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueScalar();

	/** Sets the pending value from normalized [0,1] UI space. / 从归一化 [0,1] UI 空间设置待应用值。 */
	void SetValueNormalized(double NormalizedValue);
	/** Returns the pending value mapped into normalized [0,1] UI space. / 返回映射到归一化 [0,1] UI 空间的待应用值。 */
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

	/** Returns the optional default in source-value space. / 返回源数值空间中的可选默认值。 */
	virtual TOptional<double> GetDefaultValue() const						PURE_VIRTUAL(, return TOptional<double>(););
	/** Sets the pending source-space value and reports its change reason. / 设置待应用源数值并报告变更原因。 */
	virtual void SetValue(double Value, EGSS_GameSettingChangeReason Reason = EGSS_GameSettingChangeReason::Change)	PURE_VIRTUAL(, );
	/** Returns the pending source-space value. / 返回待应用源数值。 */
	virtual double GetValue() const											PURE_VIRTUAL(, return 0;);
	/** Returns the full source-space range represented by the UI. / 返回 UI 所表示的完整源数值范围。 */
	virtual TRange<double> GetSourceRange() const							PURE_VIRTUAL(, return TRange<double>(););
	/** Returns the positive source-space quantization step. / 返回正的源数值量化步长。 */
	virtual double GetSourceStep() const									PURE_VIRTUAL(, return 0.01;);
	double GetNormalizedStepSize() const
	{
		TRange<double> SourceRange = GetSourceRange();
		return GetSourceStep() / FMath::Abs(SourceRange.GetUpperBoundValue() - SourceRange.GetLowerBoundValue());
	}
	/** Returns localized text for displaying the pending value. / 返回用于显示待应用值的本地化文本。 */
	virtual FText GetFormattedText() const									PURE_VIRTUAL(, return FText::GetEmpty(););
	
protected:
};
