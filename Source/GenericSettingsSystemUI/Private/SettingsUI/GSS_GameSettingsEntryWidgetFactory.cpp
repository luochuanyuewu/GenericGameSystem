// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingsEntryWidgetFactory.h"

#include "Settings/GSS_GameSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsEntryWidgetFactory)

TSubclassOf<UUserWidget> UGSS_GameSettingsEntryWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	const UGSS_GameSetting* Setting = Cast<UGSS_GameSetting>(Data);
	if (!Setting)
	{
		return nullptr;
	}

	TSubclassOf<UGSS_GameSetting> BestClass;
	TSoftClassPtr<UGSS_GameSettingListEntry> BestEntryClass;
	for (const TPair<TSubclassOf<UGSS_GameSetting>, TSoftClassPtr<UGSS_GameSettingListEntry>>& Pair : EntryClasses)
	{
		if (Pair.Key && Setting->IsA(Pair.Key) && (!BestClass || Pair.Key->IsChildOf(BestClass)))
		{
			BestClass = Pair.Key;
			BestEntryClass = Pair.Value;
		}
	}
	return BestEntryClass.LoadSynchronous();
}
