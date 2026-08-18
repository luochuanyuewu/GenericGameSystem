// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Settings/GSS_GameSettingFilterState.h"

#include "GSS_GameSettingEditableStateLibrary.generated.h"

/** Blueprint helpers for reading and mutating an edit-condition's state parameter. / 用于读取和修改编辑条件状态参数的蓝图辅助函数。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingEditableStateLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Edit State") static bool IsVisible(const FGSS_GameSettingEditableState& State) { return State.IsVisible(); }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Edit State") static bool IsEnabled(const FGSS_GameSettingEditableState& State) { return State.IsEnabled(); }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Edit State") static bool IsResettable(const FGSS_GameSettingEditableState& State) { return State.IsResettable(); }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Edit State") static TArray<FText> GetDisabledReasons(const FGSS_GameSettingEditableState& State) { return State.GetDisabledReasons(); }
	UFUNCTION(BlueprintPure, Category = "GSS|Settings|Edit State") static TArray<FString> GetDisabledOptions(const FGSS_GameSettingEditableState& State) { return State.GetDisabledOptions(); }

	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Edit State") static void Hide(UPARAM(ref) FGSS_GameSettingEditableState& State, const FString& DeveloperReason) { State.Hide(DeveloperReason); }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Edit State") static void Disable(UPARAM(ref) FGSS_GameSettingEditableState& State, const FText& Reason) { State.Disable(Reason); }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Edit State") static void DisableOption(UPARAM(ref) FGSS_GameSettingEditableState& State, const FString& OptionValue) { State.DisableOption(OptionValue); }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Edit State") static void PreventReset(UPARAM(ref) FGSS_GameSettingEditableState& State) { State.PreventReset(); }
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings|Edit State") static void Kill(UPARAM(ref) FGSS_GameSettingEditableState& State, const FString& DeveloperReason) { State.Kill(DeveloperReason); }
};
