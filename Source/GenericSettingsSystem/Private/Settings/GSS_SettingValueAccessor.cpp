// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#include "Settings/GSS_SettingValueAccessor.h"
#include "Settings/GSS_SettingsSubsystem.h"
#include "Engine/Engine.h"
#include "UObject/EnumProperty.h"
#include "UObject/StructOnScope.h"

UObject* FGSS_SettingValueAccessor::ResolveTarget(const FGSS_SettingValueAccessor& Accessor, ULocalPlayer* LocalPlayer)
{
	if (Accessor.Source == EGSS_SettingValueSource::Local)
	{
		return GEngine ? GEngine->GetGameUserSettings() : nullptr;
	}
	return UGSS_SettingsSubsystem::Get(LocalPlayer) ? UGSS_SettingsSubsystem::Get(LocalPlayer)->GetSharedSettings() : nullptr;
}

FProperty* FGSS_SettingValueAccessor::GetReturnProperty(UFunction* Function)
{
	for (TFieldIterator<FProperty> It(Function); It; ++It) if (It->HasAnyPropertyFlags(CPF_ReturnParm)) return *It;
	return nullptr;
}

bool FGSS_SettingValueAccessor::AreSerializedValuesEqual(const FString& Left, const FString& Right)
{
	if (Left.Equals(Right, ESearchCase::IgnoreCase))
	{
		return true;
	}

	const auto IsTrueToken = [](const FString& Value)
	{
		return Value.Equals(TEXT("true"), ESearchCase::IgnoreCase) || Value == TEXT("1");
	};
	const auto IsFalseToken = [](const FString& Value)
	{
		return Value.Equals(TEXT("false"), ESearchCase::IgnoreCase) || Value == TEXT("0");
	};

	return (IsTrueToken(Left) && IsTrueToken(Right)) || (IsFalseToken(Left) && IsFalseToken(Right));
}

bool FGSS_SettingValueAccessor::ExportCanonicalValue(FProperty* Property, const void* ValuePtr, FString& OutValue)
{
	if (!Property || !ValuePtr)
	{
		return false;
	}

	if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		OutValue = LexToString(BoolProperty->GetPropertyValue(ValuePtr));
		return true;
	}

	if (const FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		if (const UEnum* Enum = EnumProperty->GetEnum())
		{
			const int64 EnumValue = EnumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(ValuePtr);
			OutValue = Enum->GetNameStringByValue(EnumValue);
			return !OutValue.IsEmpty();
		}
		return false;
	}

	if (const FByteProperty* ByteProperty = CastField<FByteProperty>(Property); ByteProperty && ByteProperty->Enum)
	{
		OutValue = ByteProperty->Enum->GetNameStringByValue(ByteProperty->GetSignedIntPropertyValue(ValuePtr));
		return !OutValue.IsEmpty();
	}

	OutValue.Reset();
	Property->ExportTextItem_Direct(OutValue, ValuePtr, nullptr, nullptr, PPF_None);
	return true;
}

bool FGSS_SettingValueAccessor::ImportCanonicalValue(FProperty* Property, void* ValuePtr, const FString& Value)
{
	if (!Property || !ValuePtr)
	{
		return false;
	}

	if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
	{
		BoolProperty->SetPropertyValue(ValuePtr, Value.ToBool());
		return true;
	}

	if (const FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
	{
		if (const UEnum* Enum = EnumProperty->GetEnum())
		{
			int64 EnumValue = Enum->GetValueByNameString(Value);
			if (EnumValue == INDEX_NONE)
			{
				EnumValue = Enum->GetValueByNameString(Value, EGetByNameFlags::CheckAuthoredName);
			}
			if (EnumValue == INDEX_NONE)
			{
				return false;
			}
			EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(ValuePtr, EnumValue);
			return true;
		}
		return false;
	}

	if (const FByteProperty* ByteProperty = CastField<FByteProperty>(Property); ByteProperty && ByteProperty->Enum)
	{
		int64 EnumValue = ByteProperty->Enum->GetValueByNameString(Value);
		if (EnumValue == INDEX_NONE)
		{
			return false;
		}
		ByteProperty->SetIntPropertyValue(ValuePtr, EnumValue);
		return true;
	}

	return Property->ImportText_Direct(*Value, ValuePtr, nullptr, PPF_None) != nullptr;
}

bool FGSS_SettingValueAccessor::GetValue(ULocalPlayer* LocalPlayer, FString& OutValue) const
{
	if (!IsValid())
	{
		return false;
	}
	UObject* Target = ResolveTarget(*this, LocalPlayer);
	if (!Target)
	{
		return false;
	}
	UFunction* Function = Target->FindFunction(GetterFunction);
	FProperty* Return = Function ? GetReturnProperty(Function) : nullptr;
	if (!Return || Function->NumParms != 1)
	{
		return false;
	}
	FStructOnScope Params(Function);
	Target->ProcessEvent(Function, Params.GetStructMemory());
	return ExportCanonicalValue(Return, Return->ContainerPtrToValuePtr<void>(Params.GetStructMemory()), OutValue);
}

bool FGSS_SettingValueAccessor::SetValue(ULocalPlayer* LocalPlayer, const FString& Value) const
{
	if (!IsValid())
	{
		return false;
	}
	UObject* Target = ResolveTarget(*this, LocalPlayer);
	if (!Target)
	{
		return false;
	}
	UFunction* Function = Target->FindFunction(SetterFunction);
	if (!Function)
	{
		return false;
	}
	FProperty* Parameter = nullptr;
	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		if (!It->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			if (Parameter)
			{
				return false;
			}
			Parameter = *It;
		}
	}
	if (!Parameter || Function->NumParms != 1)
	{
		return false;
	}
	FStructOnScope Params(Function);
	void* ValuePtr = Parameter->ContainerPtrToValuePtr<void>(Params.GetStructMemory());
	if (!ImportCanonicalValue(Parameter, ValuePtr, Value))
	{
		return false;
	}
	Target->ProcessEvent(Function, Params.GetStructMemory());
	return true;
}
