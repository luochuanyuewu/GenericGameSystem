// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsDetailSectionBuilder.h"

#include "Settings/GSS_GameSetting.h"
#include "SettingsUI/GSS_SettingsDetailSection.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsDetailSectionBuilder)

TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> UGSS_SettingsDetailSectionBuilder::GatherDetailSections_Implementation(const UObject* Data)
{
	TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> Result;
	for (const TSoftClassPtr<UGSS_SettingsDetailSection>& SectionClass : GetSectionsForSetting(Cast<UGSS_GameSetting>(Data)))
	{
		Result.Add(SectionClass);
	}
	return Result;
}

TArray<TSoftClassPtr<UGSS_SettingsDetailSection>> UGSS_SettingsDetailSectionBuilder::GetSectionsForSetting(const UGSS_GameSetting* Setting) const
{
	TArray<TSoftClassPtr<UGSS_SettingsDetailSection>> Result;
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
