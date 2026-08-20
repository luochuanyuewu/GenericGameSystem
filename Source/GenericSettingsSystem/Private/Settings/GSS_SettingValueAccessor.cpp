// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#include "Settings/GSS_SettingValueAccessor.h"

#include "Engine/Engine.h"
#include "GenericSettingsSystem.h"
#include "Settings/GSS_SettingsDeveloperSettings.h"
#include "Settings/GSS_SettingsShared.h"
#include "Settings/GSS_GameSettingsSubsystem.h"
#include "UObject/EnumProperty.h"
#include "UObject/StructOnScope.h"

namespace GSS_Accessor
{
	static bool HasGetterPrefix(const FName FunctionName)
	{
		const FString Name = FunctionName.ToString();
		return Name.StartsWith(TEXT("Get")) || Name.StartsWith(TEXT("Is")) || Name.StartsWith(TEXT("Has"));
	}

	static bool HasSetterPrefix(const FName FunctionName)
	{
		return FunctionName.ToString().StartsWith(TEXT("Set"));
	}
}

UObject* FGSS_SettingValueAccessor::ResolveTarget(const FGSS_SettingValueAccessor& Accessor, ULocalPlayer* LocalPlayer)
{
	if (Accessor.Source == EGSS_SettingValueSource::Local)
	{
		return GEngine ? GEngine->GetGameUserSettings() : nullptr;
	}
	return UGSS_GameSettingsSubsystem::Get(LocalPlayer) ? UGSS_GameSettingsSubsystem::Get(LocalPlayer)->GetSharedSettings() : nullptr;
}

UClass* FGSS_SettingValueAccessor::ResolveTargetClass() const
{
	if (Source == EGSS_SettingValueSource::Local)
	{
		if (GEngine && GEngine->GameUserSettingsClass)
		{
			return GEngine->GameUserSettingsClass;
		}
		if (const UEngine* EngineCDO = GetDefault<UEngine>(); EngineCDO && EngineCDO->GameUserSettingsClass)
		{
			return EngineCDO->GameUserSettingsClass;
		}
		return UGameUserSettings::StaticClass();
	}

	const UGSS_SettingsDeveloperSettings* Settings = GetDefault<UGSS_SettingsDeveloperSettings>();
	if (Settings && Settings->SharedSettingsClass)
	{
		return Settings->SharedSettingsClass.Get();
	}
	return UGSS_SettingsShared::StaticClass();
}

FProperty* FGSS_SettingValueAccessor::GetReturnProperty(const UFunction* Function)
{
	if (!Function)
	{
		return nullptr;
	}
	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		if (It->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			return *It;
		}
	}
	return nullptr;
}

bool FGSS_SettingValueAccessor::IsCompatibleGetter(const UFunction* Function)
{
	return Function && Function->NumParms == 1 && GetReturnProperty(Function) != nullptr;
}

bool FGSS_SettingValueAccessor::IsCompatibleSetter(const UFunction* Function)
{
	if (!Function || Function->NumParms != 1 || GetReturnProperty(Function))
	{
		return false;
	}

	int32 ParameterCount = 0;
	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		if (!It->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			++ParameterCount;
		}
	}
	return ParameterCount == 1;
}

TArray<FString> FGSS_SettingValueAccessor::ListCompatibleFunctions(const UClass* TargetClass, bool bGetters)
{
	TArray<FString> Names;
	if (!TargetClass)
	{
		return Names;
	}

	TSet<FName> Seen;
	for (TFieldIterator<UFunction> It(TargetClass, EFieldIterationFlags::IncludeSuper); It; ++It)
	{
		const UFunction* Function = *It;
		if (!Function || Function->HasAnyFunctionFlags(FUNC_Delegate | FUNC_Static | FUNC_UbergraphFunction))
		{
			continue;
		}
		if (Function->GetOwnerClass() == UObject::StaticClass())
		{
			continue;
		}
		if (bGetters)
		{
			if (!IsCompatibleGetter(Function) || !GSS_Accessor::HasGetterPrefix(Function->GetFName()))
			{
				continue;
			}
		}
		else if (!IsCompatibleSetter(Function) || !GSS_Accessor::HasSetterPrefix(Function->GetFName()))
		{
			continue;
		}
		if (Seen.Contains(Function->GetFName()))
		{
			continue;
		}
		Seen.Add(Function->GetFName());
		Names.Add(Function->GetName());
	}
	Names.Sort();
	return Names;
}

TArray<FString> FGSS_SettingValueAccessor::GetCompatibleGetterNames() const
{
	return ListCompatibleFunctions(ResolveTargetClass(), true);
}

TArray<FString> FGSS_SettingValueAccessor::GetCompatibleSetterNames() const
{
	return ListCompatibleFunctions(ResolveTargetClass(), false);
}

bool FGSS_SettingValueAccessor::Validate(FString& OutError) const
{
	if (!IsValid())
	{
		OutError = TEXT("Accessor needs both a Getter and a Setter function name.");
		return false;
	}

	UClass* TargetClass = ResolveTargetClass();
	if (!TargetClass)
	{
		OutError = TEXT("Accessor target class could not be resolved.");
		return false;
	}

	const UFunction* Getter = TargetClass->FindFunctionByName(GetterFunction);
	if (!IsCompatibleGetter(Getter))
	{
		OutError = FString::Printf(TEXT("Accessor Getter '%s' is not a zero-argument UFUNCTION with a return value on %s."), *GetterFunction.ToString(), *TargetClass->GetName());
		return false;
	}

	const UFunction* Setter = TargetClass->FindFunctionByName(SetterFunction);
	if (!IsCompatibleSetter(Setter))
	{
		OutError = FString::Printf(TEXT("Accessor Setter '%s' is not a one-argument void UFUNCTION on %s."), *SetterFunction.ToString(), *TargetClass->GetName());
		return false;
	}

	return true;
}

FGSS_SettingValueAccessor FGSS_SettingValueAccessor::MakeLocal(FName Getter, FName Setter)
{
	FGSS_SettingValueAccessor Accessor;
	Accessor.Source = EGSS_SettingValueSource::Local;
	Accessor.GetterFunction = Getter;
	Accessor.SetterFunction = Setter;
	return Accessor;
}

FGSS_SettingValueAccessor FGSS_SettingValueAccessor::MakeShared(FName Getter, FName Setter)
{
	FGSS_SettingValueAccessor Accessor;
	Accessor.Source = EGSS_SettingValueSource::Shared;
	Accessor.GetterFunction = Getter;
	Accessor.SetterFunction = Setter;
	return Accessor;
}

void FGSS_SettingValueAccessor::LogFailure(const TCHAR* Operation, const UObject* Target, const FName FunctionName, const TCHAR* Reason) const
{
	UE_LOG(LogGSS, Warning, TEXT("Accessor %s failed on %s::%s (%s target): %s"),
		Operation,
		Target ? *Target->GetClass()->GetName() : TEXT("<none>"),
		*FunctionName.ToString(),
		Source == EGSS_SettingValueSource::Local ? TEXT("Local") : TEXT("Shared"),
		Reason);
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
		LogFailure(TEXT("GetValue"), nullptr, GetterFunction, TEXT("Getter or Setter is not configured."));
		return false;
	}
	UObject* Target = ResolveTarget(*this, LocalPlayer);
	if (!Target)
	{
		LogFailure(TEXT("GetValue"), nullptr, GetterFunction, TEXT("target object is missing."));
		return false;
	}
	UFunction* Function = Target->FindFunction(GetterFunction);
	FProperty* Return = Function ? GetReturnProperty(Function) : nullptr;
	if (!Return || Function->NumParms != 1)
	{
		LogFailure(TEXT("GetValue"), Target, GetterFunction, TEXT("function is missing or is not a zero-argument Getter."));
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
		LogFailure(TEXT("SetValue"), nullptr, SetterFunction, TEXT("Getter or Setter is not configured."));
		return false;
	}
	UObject* Target = ResolveTarget(*this, LocalPlayer);
	if (!Target)
	{
		LogFailure(TEXT("SetValue"), nullptr, SetterFunction, TEXT("target object is missing."));
		return false;
	}
	UFunction* Function = Target->FindFunction(SetterFunction);
	if (!Function)
	{
		LogFailure(TEXT("SetValue"), Target, SetterFunction, TEXT("function is missing."));
		return false;
	}
	FProperty* Parameter = nullptr;
	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		if (!It->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			if (Parameter)
			{
				LogFailure(TEXT("SetValue"), Target, SetterFunction, TEXT("function is not a one-argument Setter."));
				return false;
			}
			Parameter = *It;
		}
	}
	if (!Parameter || Function->NumParms != 1)
	{
		LogFailure(TEXT("SetValue"), Target, SetterFunction, TEXT("function is not a one-argument Setter."));
		return false;
	}
	FStructOnScope Params(Function);
	void* ValuePtr = Parameter->ContainerPtrToValuePtr<void>(Params.GetStructMemory());
	if (!ImportCanonicalValue(Parameter, ValuePtr, Value))
	{
		LogFailure(TEXT("SetValue"), Target, SetterFunction, TEXT("value could not be imported into the Setter parameter."));
		return false;
	}
	Target->ProcessEvent(Function, Params.GetStructMemory());
	return true;
}
