// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "UI/Common/GUIS_WidgetFactory.h"

#include "GSS_GameSettingsEntryWidgetFactory.generated.h"

class UGSS_GameSettingListEntry;
class UGSS_GameSetting;

/** Maps GSS runtime setting classes to Blueprint entry widgets. / 将 GSS 运行时设置类映射到蓝图条目 Widget。 */
UCLASS(NotBlueprintable, BlueprintType)
class GENERICSETTINGSSYSTEMUI_API UGSS_GameSettingsEntryWidgetFactory : public UGUIS_WidgetFactory
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;

protected:
	/** Most-specific matching setting class wins. / 最具体的匹配设置类优先。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GSS|Settings UI",meta = (AllowAbstract))
	TMap<TSubclassOf<UGSS_GameSetting>, TSoftClassPtr<UGSS_GameSettingListEntry>> EntryClasses;
};
