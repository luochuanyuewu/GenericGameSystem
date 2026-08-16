// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Misc/TextFilterExpressionEvaluator.h"

#include "UObject/ObjectPtr.h"
#include "GSS_GameSettingFilterState.generated.h"

class ULocalPlayer;
class UGSS_GameSetting;
class UGSS_GameSettingCollection;

/** Reason attached to a setting-change notification. / 设置变更通知附带的原因。 */
UENUM(BlueprintType)
enum class EGSS_GameSettingChangeReason : uint8
{
	/** A user or caller changed the pending value. / 用户或调用方修改了待应用值。 */
	Change,
	/** A dependent setting changed and this node needs reevaluation. / 依赖设置变化，本节点需要重新计算。 */
	DependencyChanged,
	/** The pending value was changed to its default. / 待应用值被改为默认值。 */
	ResetToDefault,
	/** The pending value was restored to its last applied value. / 待应用值被恢复为最后一次已应用值。 */
	RestoreToInitial,
};

/**
 * Query parameters controlling which runtime settings a UI receives.
 * 控制 UI 获取哪些运行时设置的查询参数。
 *
 * It supports availability flags, text search, an explicit root list and an optional allow list.
 * 支持可用性标志、文本搜索、显式根列表和可选允许列表。
 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEM_API FGSS_GameSettingFilterState
{
	GENERATED_BODY()

public:

	FGSS_GameSettingFilterState();

	/** Include disabled settings. / 是否包含不可用设置。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSS|Settings")
	bool bIncludeDisabled = true;

	/** Include hidden settings. / 是否包含隐藏设置。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSS|Settings")
	bool bIncludeHidden = false;

	/** Include settings that can be reset. / 是否包含可重置设置。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSS|Settings")
	bool bIncludeResetable = true;

	/** Traverse into nested pages instead of treating them as navigation boundaries. / 是否进入嵌套页面，而非将其视为导航边界。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GSS|Settings")
	bool bIncludeNestedPages = false;

public:
	/** Sets the text expression matched against setting title and description. / 设置与设置标题和说明匹配的文本表达式。 */
	void SetSearchText(const FString& InSearchText);

	/** Returns whether a setting passes every active filter. / 返回设置是否通过所有启用的筛选条件。 */
	bool DoesSettingPassFilter(const UGSS_GameSetting& InSetting) const;

	/** Adds a setting as an explicit query root. / 将设置添加为显式查询根。 */
	void AddSettingToRootList(UGSS_GameSetting* InSetting);
	/** Restricts results to this setting when any allow-list item exists. / Allow List 非空时，限制结果包含该设置。 */
	void AddSettingToAllowList(UGSS_GameSetting* InSetting);

	bool IsSettingInAllowList(const UGSS_GameSetting* InSetting) const
	{
		return SettingAllowList.Contains(InSetting);
	}
	
	const TArray<UGSS_GameSetting*>& GetSettingRootList() const { return SettingRootList; }
	bool IsSettingInRootList(const UGSS_GameSetting* InSetting) const
	{
		return SettingRootList.Contains(InSetting);
	}

private:
	FTextFilterExpressionEvaluator SearchTextEvaluator;

	UPROPERTY()
	TArray<TObjectPtr<UGSS_GameSetting>> SettingRootList;

	// If this is non-empty, then only settings in here are allowed
	UPROPERTY()
	TArray<TObjectPtr<UGSS_GameSetting>> SettingAllowList;
};
