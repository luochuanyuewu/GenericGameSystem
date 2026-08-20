// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GSS_SettingValueAccessor.h"
#include "GSS_GameSettingsDefinitions.generated.h"

class UGSS_GameSetting;
class UGSS_GameSettingsBuilder;
class UGSS_GameSettingEditCondition;

/** One selectable serialized option for a discrete setting. / 离散设置中的一个可选序列化选项。 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEM_API FGSS_DiscreteOptionDefinition
{
	GENERATED_BODY()

/** Stable value written through the Accessor. / 通过 Accessor 写入的稳定值。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FString Value;

	/** Localized label shown by a settings UI. / 设置 UI 显示的本地化标签。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText DisplayName;
};

/**
 * Authoring-time description of one runtime setting node.
 * 一个运行时设置节点的编辑期描述。
 *
 * SettingId is the global identity used for querying runtime settings. Definitions only
 * describe construction; the Registry owns the runtime node created from them.
 * SettingId 是用于查询运行时设置的全局稳定标识。Definition 只描述构建方式，运行时节点由 Registry 持有。
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, CollapseCategories)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition : public UObject
{
	GENERATED_BODY()

public:
	/** Required globally unique ID. / 必填的全局唯一 ID。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings", meta = (Categories = "GSS.Settings"))
	FGameplayTag SettingId;

	/** Localized title for UI presentation. / 用于 UI 展示的本地化标题。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText DisplayName;

	/** Localized rich-text description for UI presentation. / 用于 UI 展示的本地化富文本说明。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings", meta = (MultiLine = true))
	FText Description;

	/** Reflected Local or Shared value access. Value definitions must configure valid Getter and Setter functions. / 反射的 Local 或 Shared 值访问；值 Definition 必须配置有效 Getter 与 Setter 函数。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FGSS_SettingValueAccessor Accessor;

	/** Rules evaluated to determine visibility and availability. / 用于确定可见性与可用性的规则。 */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GSS|Settings")
	TArray<TObjectPtr<UGSS_GameSettingEditCondition>> EditConditions;

	/** Compatible Getter names for the current Accessor source. / 当前 Accessor 源上签名匹配的 Getter 名。 */
	UFUNCTION()
	TArray<FString> GetAccessorGetterFunctionOptions() const { return Accessor.GetCompatibleGetterNames(); }
	/** Compatible Setter names for the current Accessor source. / 当前 Accessor 源上签名匹配的 Setter 名。 */
	UFUNCTION()
	TArray<FString> GetAccessorSetterFunctionOptions() const { return Accessor.GetCompatibleSetterNames(); }
};

/** Groups child definitions; pages are navigable collections while regular collections are inline groups. / 组织子 Definition；Page 为可导航集合，普通 Collection 为内联分组。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition_Collection : public UGSS_GameSettingDefinition
{
	GENERATED_BODY()

public:
	/** Child nodes in display/build order. / 按显示与构建顺序排列的子节点。 */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GSS|Settings")
	TArray<TObjectPtr<UGSS_GameSettingDefinition>> Children;

	/** Whether this collection is a separately navigated page. / 此集合是否为单独导航的页面。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	bool bIsPage = false;

	/** Label used by navigation UI when bIsPage is true. / bIsPage 为 true 时导航 UI 使用的标签。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings", meta = (EditCondition = "bIsPage"))
	FText NavigationText;
};

/** Boolean setting definition, persisted as "true" or "false". / 布尔设置定义，以 "true" 或 "false" 持久化。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition_Bool : public UGSS_GameSettingDefinition
{
	GENERATED_BODY()

public:
/** Value used when the Accessor has no applied value yet. / Accessor 尚无已应用值时使用的默认值。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	bool DefaultValue = false;
};

/** Setting definition with a finite list of string-backed options. / 具有有限字符串选项列表的设置定义。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition_Discrete : public UGSS_GameSettingDefinition
{
	GENERATED_BODY()

public:
	/** Serialized default value; it should match an entry in Options. / 序列化默认值；应与 Options 中的一个条目匹配。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FString DefaultValue;

	/** Ordered choices exposed to the UI. / 提供给 UI 的有序选项。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	TArray<FGSS_DiscreteOptionDefinition> Options;
};

/** Built-in Scalar UI formats that Data Assets and Blueprint Builders can select. / Data Asset 与蓝图 Builder 可选择的内置 Scalar UI 格式。 */
UENUM(BlueprintType)
enum class EGSS_SettingScalarDisplayFormat : uint8
{
	/** Source value with default numeric formatting. / 使用默认数值格式显示源值。 */
	Raw UMETA(DisplayName = "Raw"),
	/** Source value with one fractional digit. / 源值，保留一位小数。 */
	RawOneDecimal UMETA(DisplayName = "Raw (One Decimal)"),
	/** Source value with two fractional digits. / 源值，保留两位小数。 */
	RawTwoDecimals UMETA(DisplayName = "Raw (Two Decimals)"),
	/** Normalized [0,1] value shown as an integer percent. / 将归一化 [0,1] 显示为整数百分比。 */
	ZeroToOnePercent UMETA(DisplayName = "Normalized Percent"),
	/** Normalized [0,1] value shown as a percent with one decimal. / 将归一化 [0,1] 显示为一位小数百分比。 */
	ZeroToOnePercentOneDecimal UMETA(DisplayName = "Normalized Percent (One Decimal)"),
	/** Source value in 0–1 shown as an integer percent. / 将 0–1 的源值显示为整数百分比。 */
	SourceAsPercent1 UMETA(DisplayName = "Source as Percent (0-1)"),
	/** Source value in 0–100 shown as an integer percent. / 将 0–100 的源值显示为整数百分比。 */
	SourceAsPercent100 UMETA(DisplayName = "Source as Percent (0-100)"),
	/** Source value rounded to an integer. / 将源值四舍五入为整数。 */
	SourceAsInteger UMETA(DisplayName = "Source as Integer")
};

/** Numeric setting definition with a source range and quantization step. / 具有取值范围与量化步长的数值设置定义。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition_Scalar : public UGSS_GameSettingDefinition
{
	GENERATED_BODY()

public:
	/** Default source-space value. / 源数值空间中的默认值。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	double DefaultValue = 0.0;

	/** Inclusive lower bound of the source range. / 源数值范围的包含下界。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	double MinimumValue = 0.0;

	/** Inclusive upper bound of the source range. / 源数值范围的包含上界。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	double MaximumValue = 1.0;

	/** Positive increment used to quantize edits. / 用于量化编辑的正增量。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings", meta = (ClampMin = "0.000001"))
	double Step = 0.01;

	/** How the pending value is formatted in settings UI. / 设置 UI 如何格式化待应用值。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	EGSS_SettingScalarDisplayFormat DisplayFormat = EGSS_SettingScalarDisplayFormat::Raw;
};

/** Invokable command definition; actions do not represent a persisted preference by default. / 可调用命令定义；Action 默认不表示可持久化偏好。 */
UCLASS(BlueprintType, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingDefinition_Action : public UGSS_GameSettingDefinition
{
	GENERATED_BODY()

public:
	/** Localized text displayed by the action control. / Action 控件显示的本地化文本。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FText ActionText;

	/** Optional tag emitted when the action executes. / Action 执行时发出的可选 Tag。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings")
	FGameplayTag ActionId;
};

/**
	 * Root Data Asset for statically authored settings.
 * 静态设置的根 Data Asset。
 *
 * Assign this asset in Generic Settings System project settings; each LocalPlayer builds an independent runtime Registry from it.
 * 请在 Generic Settings System 项目设置中指定该资产；每个 LocalPlayer 都会从它构建独立的运行时 Registry。
 */
UCLASS(BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingsDefinitionAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Builds every root definition using Builder. / 使用 Builder 构建全部根 Definition。 */
	void Build(UGSS_GameSettingsBuilder* Builder) const;

	/** Root nodes in display/build order. / 按显示与构建顺序排列的根节点。 */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GSS|Settings")
	TArray<TObjectPtr<UGSS_GameSettingDefinition>> RootSettings;
};
