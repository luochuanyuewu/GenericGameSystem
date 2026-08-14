// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValueDiscrete.h"

#include "GSS_GameSettingValueDiscreteDynamic.generated.h"

class FGSS_GameSettingDataSource;
enum class EGSS_GameSettingChangeReason : uint8;

struct FContentControlsRules;

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic : public UGSS_GameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:
	UGSS_GameSettingValueDiscreteDynamic();

	/** UGSS_GameSettingValue */
	virtual void Startup() override;
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGSS_GameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual int32 GetDiscreteOptionDefaultIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

	/** UGSS_GameSettingValueDiscreteDynamic */
	void SetDynamicGetter(const TSharedRef<FGSS_GameSettingDataSource>& InGetter);
	void SetDynamicSetter(const TSharedRef<FGSS_GameSettingDataSource>& InSetter);
	void SetDefaultValueFromString(FString InOptionValue);
	void AddDynamicOption(FString InOptionValue, FText InOptionText);
	void RemoveDynamicOption(FString InOptionValue);
	const TArray<FString>& GetDynamicOptions();

	bool HasDynamicOption(const FString& InOptionValue);

	FString GetValueAsString() const;
	void SetValueFromString(FString InStringValue);

protected:
	void SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason);

	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;

	void OnDataSourcesReady();

	bool AreOptionsEqual(const FString& InOptionA, const FString& InOptionB) const;

protected:
	TSharedPtr<FGSS_GameSettingDataSource> Getter;
	TSharedPtr<FGSS_GameSettingDataSource> Setter;

	TOptional<FString> DefaultValue;
	FString InitialValue;

	TArray<FString> OptionValues;
	TArray<FText> OptionDisplayTexts;
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Bool
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Bool : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscreteDynamic_Bool();

public:
	void SetDefaultValue(bool Value);

	void SetTrueText(const FText& InText);
	void SetFalseText(const FText& InText);

#if !UE_BUILD_SHIPPING
	void SetTrueText(const FString& Value) { SetTrueText(FText::FromString(Value)); }
	void SetFalseText(const FString& Value) { SetFalseText(FText::FromString(Value)); }
#endif
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Number
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Number : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscreteDynamic_Number();

public:
	template<typename NumberType>
	void SetDefaultValue(NumberType InValue)
	{
		SetDefaultValueFromString(LexToString(InValue));
	}

	template<typename NumberType>
	void AddOption(NumberType InValue, const FText& InOptionText)
	{
		AddDynamicOption(LexToString(InValue), InOptionText);
	}

	template<typename NumberType>
	NumberType GetValue() const
	{
		const FString ValueString = GetValueAsString();

		NumberType OutValue;
		LexFromString(OutValue, *ValueString);

		return OutValue;
	}

	template<typename NumberType>
	void SetValue(NumberType InValue)
	{
		SetValueFromString(LexToString(InValue));
	}

protected:
	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Enum
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Enum : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscreteDynamic_Enum();

public:
	template<typename EnumType>
	void SetDefaultValue(EnumType InEnumValue)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue((int64)InEnumValue);
		SetDefaultValueFromString(StringValue);
	}

	template<typename EnumType>
	void AddEnumOption(EnumType InEnumValue, const FText& InOptionText)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue((int64)InEnumValue);
		AddDynamicOption(StringValue, InOptionText);
	}

	template<typename EnumType>
	EnumType GetValue() const
	{
		const FString Value = GetValueAsString();
		return (EnumType)StaticEnum<EnumType>()->GetValueByNameString(Value);
	}

	template<typename EnumType>
	void SetValue(EnumType InEnumValue)
	{
		const FString StringValue = StaticEnum<EnumType>()->GetNameStringByValue((int64)InEnumValue);
		SetValueFromString(StringValue);
	}

protected:
	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Color
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Color : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscreteDynamic_Color();

public:
	void SetDefaultValue(FLinearColor InColor)
	{
		SetDefaultValueFromString(InColor.ToString());
	}

	void AddColorOption(FLinearColor InColor)
	{
		const FColor SRGBColor = InColor.ToFColor(true);
		AddDynamicOption(InColor.ToString(), FText::FromString(FString::Printf(TEXT("#%02X%02X%02X"), SRGBColor.R, SRGBColor.G, SRGBColor.B)));
	}

	FLinearColor GetValue() const
	{
		const FString Value = GetValueAsString();
		
		FLinearColor ColorValue;
		bool bSuccess = ColorValue.InitFromString(Value);
		ensure(bSuccess);

		return ColorValue;
	}

	void SetValue(FLinearColor InColor)
	{
		SetValueFromString(InColor.ToString());
	}
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Vector2D
//////////////////////////////////////////////////////////////////////////

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Vector2D : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:

	UGSS_GameSettingValueDiscreteDynamic_Vector2D() { }

	void SetDefaultValue(const FVector2D& InValue)
	{	
		SetDefaultValueFromString(InValue.ToString());
	}

	FVector2D GetValue() const
	{
		FVector2D ValueVector;
		ValueVector.InitFromString(GetValueAsString());
		return ValueVector;
	}

	void SetValue(const FVector2D& InValue)
	{
		SetValueFromString(InValue.ToString());
	}
};
