// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "SettingsUI/GSS_SettingsListEntry.h"

#include "GSS_SettingsValueEntries.generated.h"

/** Blueprint base for a discrete (including bool) GSS setting row. / 离散值（包括布尔值）GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick, Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Discrete : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<FText> GetOptions() const;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	int32 GetSelectedOptionIndex() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool SelectOption(int32 OptionIndex);
};

/** Blueprint base for a scalar GSS setting row. / 标量 GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Scalar : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	double GetNormalizedValue() const;
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetFormattedValue() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool SetNormalizedValue(double Value);
};

/** Blueprint base for a command GSS setting row. / 命令型 GSS 设置行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Action : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetActionText() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool ExecuteAction();
};

/** Blueprint base for a navigable GSS settings page row. / 可导航 GSS 设置页面行的蓝图基类。 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick,Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsListEntry_Navigation : public UGSS_SettingsListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FText GetNavigationText() const;
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool Navigate();
};
