// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#include "Settings/GSS_SettingValueAccessor.h"
#include "Settings/GSS_SettingsSubsystem.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
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

bool FGSS_SettingValueAccessor::GetValue(ULocalPlayer* LocalPlayer, FString& OutValue) const
{
	if (!IsValid()) return false;
	UObject* Target = ResolveTarget(*this, LocalPlayer); if (!Target) return false;
	UFunction* Function = Target->FindFunction(GetterFunction); FProperty* Return = Function ? GetReturnProperty(Function) : nullptr;
	if (!Return || Function->NumParms != 1) return false;
	FStructOnScope Params(Function); Target->ProcessEvent(Function, Params.GetStructMemory());
	Return->ExportTextItem_Direct(OutValue, Return->ContainerPtrToValuePtr<void>(Params.GetStructMemory()), nullptr, Target, PPF_None);
	return true;
}

bool FGSS_SettingValueAccessor::SetValue(ULocalPlayer* LocalPlayer, const FString& Value) const
{
	if (!IsValid()) return false;
	UObject* Target = ResolveTarget(*this, LocalPlayer); if (!Target) return false;
	UFunction* Function = Target->FindFunction(SetterFunction); if (!Function) return false;
	FProperty* Parameter = nullptr; for (TFieldIterator<FProperty> It(Function); It; ++It) if (!It->HasAnyPropertyFlags(CPF_ReturnParm)) { if (Parameter) return false; Parameter = *It; }
	if (!Parameter || Function->NumParms != 1) return false;
	FStructOnScope Params(Function); void* ValuePtr = Parameter->ContainerPtrToValuePtr<void>(Params.GetStructMemory());
	return Parameter->ImportText_Direct(*Value, ValuePtr, Target, PPF_None) != nullptr && (Target->ProcessEvent(Function, Params.GetStructMemory()), true);
}
