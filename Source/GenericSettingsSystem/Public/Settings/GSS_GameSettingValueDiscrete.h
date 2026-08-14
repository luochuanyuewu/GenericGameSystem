// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValue.h"

#include "GSS_GameSettingValueDiscrete.generated.h"

class UObject;
struct FFrame;

UCLASS(Abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscrete : public UGSS_GameSettingValue
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscrete();

	/** UGSS_GameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) PURE_VIRTUAL(,);

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual int32 GetDiscreteOptionIndex() const PURE_VIRTUAL(, return INDEX_NONE;);

	/** Optional */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual int32 GetDiscreteOptionDefaultIndex() const { return INDEX_NONE; }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual TArray<FText> GetDiscreteOptions() const PURE_VIRTUAL(, return TArray<FText>(););

	virtual FString GetAnalyticsValue() const;
};
