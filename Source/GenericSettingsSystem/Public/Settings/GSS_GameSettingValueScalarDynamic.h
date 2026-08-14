// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValueScalar.h"

#include "GSS_GameSettingValueScalarDynamic.generated.h"

struct FNumberFormattingOptions;

class FGSS_GameSettingDataSource;
class UObject;

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueScalarDynamic
//////////////////////////////////////////////////////////////////////////

typedef TFunction<FText(double SourceValue, double NormalizedValue)> FSettingScalarFormatFunction;

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueScalarDynamic : public UGSS_GameSettingValueScalar
{
	GENERATED_BODY()

public:
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

	/** UGSS_GameSettingValue */
	virtual void Startup() override;
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

	/** UGSS_GameSettingValueDiscreteDynamic */
	void SetDynamicGetter(const TSharedRef<FGSS_GameSettingDataSource>& InGetter);
	void SetDynamicSetter(const TSharedRef<FGSS_GameSettingDataSource>& InSetter);
	void SetDefaultValue(double InValue);

	/**  */
	void SetDisplayFormat(FSettingScalarFormatFunction InDisplayFormat);
	
	/**  */
	void SetSourceRangeAndStep(const TRange<double>& InRange, double InSourceStep);
	
	/**
	 * The SetSourceRangeAndStep defines the actual range the numbers could move in, but often
	 * the true minimum for the user is greater than the minimum source range, so for example, the range
	 * of some slider might be 0..100, but you want to restrict the slider so that while it shows 
	 * a bar that travels from 0 to 100, the user can't set anything lower than some minimum, e.g. 1.
	 * That is the Minimum Limit.
	 */
	void SetMinimumLimit(const TOptional<double>& InMinimum);

	/**
	 * The SetSourceRangeAndStep defines the actual range the numbers could move in, but rarely
	 * the true maximum for the user is less than the maximum source range, so for example, the range
	 * of some slider might be 0..100, but you want to restrict the slider so that while it shows
	 * a bar that travels from 0 to 100, the user can't set anything lower than some maximum, e.g. 95.
	 * That is the Maximum Limit.
	 */
	void SetMaximumLimit(const TOptional<double>& InMaximum);
	
protected:
	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;

	void OnDataSourcesReady();

protected:

	TSharedPtr<FGSS_GameSettingDataSource> Getter;
	TSharedPtr<FGSS_GameSettingDataSource> Setter;

	TOptional<double> DefaultValue;
	double InitialValue = 0;

	TRange<double> SourceRange = TRange<double>(0, 1);
	double SourceStep = 0.01;
	TOptional<double> Minimum;
	TOptional<double> Maximum;

	FSettingScalarFormatFunction DisplayFormat;
};
