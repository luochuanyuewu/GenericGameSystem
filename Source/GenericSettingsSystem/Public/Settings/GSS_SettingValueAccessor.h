// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GSS_SettingValueAccessor.generated.h"

class ULocalPlayer;

/** Selects one of the two built-in settings objects. / 选择两个内置设置对象之一。 */
UENUM(BlueprintType)
enum class EGSS_SettingValueSource : uint8
{
	/** Machine-specific engine settings. / 与设备相关的引擎设置。 */
	Local,
	/** Per-local-player portable preferences. / 每个本地玩家可携带的偏好。 */
	Shared
};

/** Lightweight reflected value access description. / 轻量的反射值访问描述。 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEM_API FGSS_SettingValueAccessor
{
	GENERATED_BODY()

	/** Object that owns the reflected functions. / 拥有反射函数的对象。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GSS|Settings") EGSS_SettingValueSource Source = EGSS_SettingValueSource::Shared;
	/** Zero-argument UFUNCTION with one return value. / 无参数且具有一个返回值的 UFUNCTION。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GSS|Settings") FName GetterFunction;
	/** One-argument, void UFUNCTION. / 单参数、无返回值的 UFUNCTION。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GSS|Settings") FName SetterFunction;

	/** Returns whether both reflected functions were configured. / 返回两个反射函数是否都已配置。 */
	bool IsValid() const { return !GetterFunction.IsNone() && !SetterFunction.IsNone(); }
	bool GetValue(ULocalPlayer* LocalPlayer, FString& OutValue) const;
	bool SetValue(ULocalPlayer* LocalPlayer, const FString& Value) const;

private:
	static UObject* ResolveTarget(const FGSS_SettingValueAccessor& Accessor, ULocalPlayer* LocalPlayer);
	static FProperty* GetReturnProperty(UFunction* Function);
};
