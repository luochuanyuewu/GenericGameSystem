// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/GUIS_DetailSectionsBuilder.h"
#include "GSS_SettingsDetailSectionBuilder.generated.h"

class UGSS_GameSetting;
class UGSS_SettingsDetailSection;

/**
 * Detail Section classes contributed by one GSS runtime setting class.
 * 一个 GSS 运行时设置类所贡献的详情 Section 类。
 */
USTRUCT(BlueprintType)
struct GENERICSETTINGSSYSTEMUI_API FGSS_SettingsDetailSectionClasses
{
	GENERATED_BODY()

	/** Section Blueprints shown when this class matches the current Setting. / 当前 Setting 匹配该类时展示的 Section 蓝图。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings UI")
	TArray<TSoftClassPtr<UGSS_SettingsDetailSection>> Sections;
};

/**
 * Maps GSS runtime setting classes to detail Sections for UGSS_SettingsDetailView.
 * 将 GSS 运行时设置类映射到 UGSS_SettingsDetailView 的详情 Section。
 *
 * The current setting's class and every setting-class superclass are checked. Sections from the most-specific
 * class are returned first, followed by Sections contributed by its base classes.
 * 会检查当前设置的类及其全部设置类父类。最具体类的 Section 会最先返回，之后依次追加父类贡献的 Section。
 */
UCLASS(NotBlueprintable, BlueprintType)
class GENERICSETTINGSSYSTEMUI_API UGSS_SettingsDetailSectionBuilder : public UGUIS_DetailSectionsBuilder
{
	GENERATED_BODY()

public:
	virtual TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> GatherDetailSections_Implementation(const UObject* Data) override;

	/** Returns Sections configured for the supplied GSS setting. / 返回为指定 GSS 设置配置的 Section。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<TSoftClassPtr<UGSS_SettingsDetailSection>> GetSectionsForSetting(const UGSS_GameSetting* Setting) const;

protected:
	/** Detail Section configurations keyed by GSS setting class. / 以 GSS 设置类为键的详情 Section 配置。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings UI", meta = (AllowAbstract))
	TMap<TSubclassOf<UGSS_GameSetting>, FGSS_SettingsDetailSectionClasses> SectionsForClasses;
};
