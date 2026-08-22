// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValueDiscrete.h"
#include "GSS_SettingValueAccessor.h"

#include "GSS_GameSettingValueDiscreteDynamic.generated.h"

enum class EGSS_GameSettingChangeReason : uint8;

struct FContentControlsRules;

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic
//////////////////////////////////////////////////////////////////////////

/** String-backed discrete value populated at runtime and committed through an Accessor. / 在运行时填充、并通过 Accessor 提交的字符串离散值。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic : public UGSS_GameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:
	UGSS_GameSettingValueDiscreteDynamic();

	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGSS_GameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual int32 GetDiscreteOptionDefaultIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

	/** Assigns the bridge used to load and commit the serialized value. / 指定用于加载与提交序列化值的桥接。 */
	void SetAccessor(const FGSS_SettingValueAccessor& InAccessor);
	/** Sets the fallback value used when Accessor has no applied value. / 设置 Accessor 无已应用值时使用的回退值。 */
	void SetDefaultValueFromString(FString InOptionValue);
	/** Adds an option and its localized display label. / 添加选项及其本地化显示标签。 */
	void AddDynamicOption(FString InOptionValue, FText InOptionText);
	/** Removes an option by serialized value. / 按序列化值移除选项。 */
	void RemoveDynamicOption(FString InOptionValue);
	/** Returns serialized option values in UI order. / 按 UI 顺序返回选项的序列化值。 */
	const TArray<FString>& GetDynamicOptions();

	/** Returns whether the serialized option exists. / 返回该序列化选项是否存在。 */
	bool HasDynamicOption(const FString& InOptionValue);

	/** Returns the pending serialized value for immediate preview. / 返回可即时预览的待应用序列化值。 */
	FString GetValueAsString() const;
	/** Changes the pending serialized value; Apply commits it through its Accessor. / 修改待应用序列化值；Apply 会通过 Accessor 提交。 */
	void SetValueFromString(FString InStringValue);

protected:
	void SetValueFromString(FString InStringValue, EGSS_GameSettingChangeReason Reason);

	/** UGSS_GameSettingValue */
	virtual void OnInitialized() override;
	virtual void OnApply() override;

	bool AreOptionsEqual(const FString& InOptionA, const FString& InOptionB) const;
	/** Returns raw option indices which remain available after edit-condition evaluation. / 返回编辑条件评估后仍可用的原始选项索引。 */
	TArray<int32> GetEnabledOptionIndices() const;
	/** Maps a raw option index to its visible UI index. / 将原始选项索引映射为可见 UI 索引。 */
	int32 GetEnabledOptionIndex(int32 RawOptionIndex) const;

protected:
	FGSS_SettingValueAccessor Accessor;

	TOptional<FString> DefaultValue;
	FString InitialValue;
	FString PendingValue;

	TArray<FString> OptionValues;
	TArray<FText> OptionDisplayTexts;
};

//////////////////////////////////////////////////////////////////////////
// UGSS_GameSettingValueDiscreteDynamic_Bool
//////////////////////////////////////////////////////////////////////////

/** Convenience boolean specialization with true/false options. / 带有 true/false 选项的布尔便捷特化。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscreteDynamic_Bool : public UGSS_GameSettingValueDiscreteDynamic
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscreteDynamic_Bool();

public:
	/** Sets the fallback boolean value. / 设置回退布尔值。 */
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

/** Template-friendly numeric option specialization. / 适合模板化数值选项的特化。 */
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

/** Template-friendly UEnum option specialization storing enum names. / 存储枚举名称、适合 UEnum 的模板化特化。 */
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

/** FLinearColor option specialization using UE string serialization. / 使用 UE 字符串序列化的 FLinearColor 选项特化。 */
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

/** FVector2D value specialization using UE string serialization. / 使用 UE 字符串序列化的 FVector2D 值特化。 */
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
