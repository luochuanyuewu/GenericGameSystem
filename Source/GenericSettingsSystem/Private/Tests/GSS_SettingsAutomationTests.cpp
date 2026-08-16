// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_SettingValueAccessor.h"
#include "Settings/GSS_SettingsShared.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsRegistryIdTest, "GSS.Settings.Registry.StableIdLookup", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsRegistryIdTest::RunTest(const FString& Parameters)
{
	UGSS_GameSettingRegistry* Registry = NewObject<UGSS_GameSettingRegistry>();
	UGSS_GameSettingValueDiscreteDynamic_Bool* Setting = NewObject<UGSS_GameSettingValueDiscreteDynamic_Bool>(Registry);
	const FGameplayTag SettingId = FGameplayTag::RequestGameplayTag(TEXT("GSS.Settings.Test.Boolean"), false);
	Setting->SetSettingId(SettingId);
	Setting->SetDevName(SettingId.GetTagName());
	Registry->RegisterSetting(Setting);
	TestEqual(TEXT("Registry resolves its stable setting id"), Registry->FindSettingById(SettingId), static_cast<UGSS_GameSetting*>(Setting));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsSharedTest, "GSS.Settings.SharedSettings.BaseClass", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsSharedTest::RunTest(const FString& Parameters)
{
	UGSS_SettingsShared* SharedSettings = NewObject<UGSS_SettingsShared>();
	TestNotNull(TEXT("Shared settings use the LocalPlayerSaveGame base"), SharedSettings);
	TestTrue(TEXT("Shared settings are a LocalPlayerSaveGame"), SharedSettings->IsA<ULocalPlayerSaveGame>());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsAccessorContractTest, "GSS.Settings.Accessor.Contract", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsAccessorContractTest::RunTest(const FString& Parameters)
{
	FGSS_SettingValueAccessor Accessor;
	TestFalse(TEXT("An Accessor without both functions is invalid"), Accessor.IsValid());
	Accessor.Source = EGSS_SettingValueSource::Shared;
	Accessor.GetterFunction = TEXT("GetExampleValue");
	Accessor.SetterFunction = TEXT("SetExampleValue");
	TestTrue(TEXT("A Local or Shared Accessor needs both reflected functions"), Accessor.IsValid());
	return true;
}

#endif
