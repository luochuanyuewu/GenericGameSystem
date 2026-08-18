// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSettingValue.h"

#include "GSS_GameSettingValueDiscrete.generated.h"

class UObject;
struct FFrame;

/** Abstract value whose UI exposes a finite set of selectable options. / UI 提供有限可选项的抽象设置值。 */
UCLASS(Abstract)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingValueDiscrete : public UGSS_GameSettingValue
{
	GENERATED_BODY()

public:
	UGSS_GameSettingValueDiscrete();

	/** Selects an option by its current UI index. / 按当前 UI 索引选择一个选项。 */
	virtual void SetDiscreteOptionByIndex(int32 Index) PURE_VIRTUAL(,);

	/** Returns the current option's UI index. / 返回当前选项的 UI 索引。 */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual int32 GetDiscreteOptionIndex() const PURE_VIRTUAL(, return INDEX_NONE;);

	/** Returns the default option index, or INDEX_NONE when no default is available. / 返回默认选项索引；没有默认值时返回 INDEX_NONE。 */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual int32 GetDiscreteOptionDefaultIndex() const { return INDEX_NONE; }

	/** Returns localized display text for selectable options. / 返回可选择选项的本地化显示文本。 */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	virtual TArray<FText> GetDiscreteOptions() const PURE_VIRTUAL(, return TArray<FText>(););

};
