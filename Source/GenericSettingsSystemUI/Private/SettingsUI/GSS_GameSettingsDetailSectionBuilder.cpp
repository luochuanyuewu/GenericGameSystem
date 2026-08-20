// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingsDetailSectionBuilder.h"

#include "Settings/GSS_GameSetting.h"
#include "SettingsUI/GSS_GameSettingsDetailSection.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsDetailSectionBuilder)

TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> UGSS_GameSettingsDetailSectionBuilder::GatherDetailSections_Implementation(const UObject* Data)
{
	TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> Result;
	for (const TSoftClassPtr<UGSS_GameSettingsDetailSection>& SectionClass : GetSectionsForSetting(Cast<UGSS_GameSetting>(Data)))
	{
		Result.Add(SectionClass);
	}
	return Result;
}

TArray<TSoftClassPtr<UGSS_GameSettingsDetailSection>> UGSS_GameSettingsDetailSectionBuilder::GetSectionsForSetting(const UGSS_GameSetting* Setting) const
{
	TArray<TSoftClassPtr<UGSS_GameSettingsDetailSection>> Result;
	if (!Setting)
	{
		return Result;
	}

	for (const UClass* SettingClass = Setting->GetClass(); SettingClass && SettingClass->IsChildOf(UGSS_GameSetting::StaticClass()); SettingClass = SettingClass->GetSuperClass())
	{
		const TSubclassOf<UGSS_GameSetting> SettingClassKey(const_cast<UClass*>(SettingClass));
		if (const FGSS_SettingsDetailSectionClasses* ConfiguredSections = SectionsForClasses.Find(SettingClassKey))
		{
			Result.Append(ConfiguredSections->Sections);
		}
	}
	return Result;
}
