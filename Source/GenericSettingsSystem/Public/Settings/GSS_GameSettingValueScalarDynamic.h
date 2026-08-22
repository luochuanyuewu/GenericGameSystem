// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValueScalar.h"
#include "GSS_SettingValueAccessor.h"

#include "GSS_GameSettingValueScalarDynamic.generated.h"

enum class EGSS_SettingScalarDisplayFormat : uint8;
struct FNumberFormattingOptions;

class UObject;

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueScalarDynamic
//////////////////////////////////////////////////////////////////////////

/** Formats a source-space and normalized value for UI display. / 将源数值和归一化值格式化为 UI 文本。 */
typedef TFunction<FText(double SourceValue, double NormalizedValue)> FSettingScalarFormatFunction;

/** Numeric value with runtime range that writes through its Accessor immediately. / 具有运行时范围、变更时立即通过 Accessor 写入的数值。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueScalarDynamic : public UGSS_GameSettingValueScalar
{
	GENERATED_BODY()

public:
	/** Built-in display formatters for common numeric presentations. / 常用数值展示的内置格式化器。 */
	static FSettingScalarFormatFunction Raw;
	static FSettingScalarFormatFunction RawOneDecimal;
	static FSettingScalarFormatFunction RawTwoDecimals;
	static FSettingScalarFormatFunction ZeroToOnePercent;
	static FSettingScalarFormatFunction ZeroToOnePercent_OneDecimal;
	static FSettingScalarFormatFunction SourceAsPercent1;
	static FSettingScalarFormatFunction SourceAsPercent100;
	static FSettingScalarFormatFunction SourceAsInteger;
private:
	static const FNumberFormattingOptions& GetOneDecimalFormattingOptions();
	
public:
	UGSS_GameSettingValueScalarDynamic();

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGSS_GameSettingValueScalar */
	virtual TOptional<double> GetDefaultValue() const override;
	virtual void SetValue(double Value, EGSS_GameSettingChangeReason Reason = EGSS_GameSettingChangeReason::Change) override;
	virtual double GetValue() const override;
	virtual TRange<double> GetSourceRange() const override;
	virtual double GetSourceStep() const override;
	virtual FText GetFormattedText() const override;

	/** Assigns the bridge used to read and write this value immediately. / 指定用于立即读写该值的桥接。 */
	void SetAccessor(const FGSS_SettingValueAccessor& InAccessor);
	/** Sets the fallback source-space value. / 设置回退源数值。 */
	void SetDefaultValue(double InValue);

	/** Selects the function used by GetFormattedText. / 指定 GetFormattedText 所使用的函数。 */
	void SetDisplayFormat(FSettingScalarFormatFunction InDisplayFormat);
	/** Resolves a Data Asset / Blueprint format enum to a built-in formatter. / 将 Data Asset / 蓝图格式枚举解析为内置格式化器。 */
	static FSettingScalarFormatFunction GetBuiltInDisplayFormat(EGSS_SettingScalarDisplayFormat Format);
	
	/** Sets the complete source range and positive edit step. / 设置完整源数值范围及正编辑步长。 */
	void SetSourceRangeAndStep(const TRange<double>& InRange, double InSourceStep);
	
	/** Sets an optional lower value limit within the source range. / 在源范围内设置可选的较低数值限制。 */
	void SetMinimumLimit(const TOptional<double>& InMinimum);

	/** Sets an optional upper value limit within the source range. / 在源范围内设置可选的较高数值限制。 */
	void SetMaximumLimit(const TOptional<double>& InMaximum);
	
protected:
	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;

	/** Quantizes and clamps a source-space value. / 量化并限制源数值。 */
	double SanitizeSourceValue(double InValue) const;

protected:

	FGSS_SettingValueAccessor Accessor;

	TOptional<double> DefaultValue;
	double InitialValue = 0;

	TRange<double> SourceRange = TRange<double>(0, 1);
	double SourceStep = 0.01;
	TOptional<double> Minimum;
	TOptional<double> Maximum;

	FSettingScalarFormatFunction DisplayFormat;
};
