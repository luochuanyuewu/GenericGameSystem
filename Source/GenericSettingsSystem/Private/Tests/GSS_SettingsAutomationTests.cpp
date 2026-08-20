// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "NativeGameplayTags.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_SettingValueAccessor.h"
#include "Settings/GSS_GameSettingsDefinitions.h"
#include "Settings/GSS_GameSettingsProvider.h"
#include "Settings/GSS_SettingsShared.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Test_Scalar, "GSS.Settings.Test.Scalar");

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

	TestTrue(TEXT("Bool tokens compare equal regardless of case"), FGSS_SettingValueAccessor::AreSerializedValuesEqual(TEXT("true"), TEXT("True")));
	TestTrue(TEXT("Bool true also matches 1"), FGSS_SettingValueAccessor::AreSerializedValuesEqual(TEXT("true"), TEXT("1")));
	TestTrue(TEXT("Bool false matches False and 0"), FGSS_SettingValueAccessor::AreSerializedValuesEqual(TEXT("False"), TEXT("0")));
	TestTrue(TEXT("Enum names compare case-insensitively"), FGSS_SettingValueAccessor::AreSerializedValuesEqual(TEXT("WindowedFullscreen"), TEXT("windowedfullscreen")));
	TestFalse(TEXT("Different enum names are not equal"), FGSS_SettingValueAccessor::AreSerializedValuesEqual(TEXT("Fullscreen"), TEXT("Windowed")));

	FGSS_SettingValueAccessor LocalVSync = GSS_MAKE_LOCAL_ACCESSOR(IsVSyncEnabled, SetVSyncEnabled);
	FString ValidationError;
	TestTrue(TEXT("Local VSync Accessor validates against GameUserSettings"), LocalVSync.Validate(ValidationError));
	TestTrue(TEXT("Compatible getters include IsVSyncEnabled"), LocalVSync.GetCompatibleGetterNames().Contains(TEXT("IsVSyncEnabled")));
	TestTrue(TEXT("Compatible setters include SetVSyncEnabled"), LocalVSync.GetCompatibleSetterNames().Contains(TEXT("SetVSyncEnabled")));

	FGSS_SettingValueAccessor MissingFunctions = FGSS_SettingValueAccessor::MakeLocal(TEXT("GetDoesNotExist"), TEXT("SetDoesNotExist"));
	TestFalse(TEXT("Missing Local functions fail Validate"), MissingFunctions.Validate(ValidationError));
	TestFalse(TEXT("Validate reports a reason when functions are missing"), ValidationError.IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsDiscreteValueContractTest, "GSS.Settings.Discrete.SerializedValueContract", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsDiscreteValueContractTest::RunTest(const FString& Parameters)
{
	UGSS_GameSettingValueDiscreteDynamic_Bool* BoolSetting = NewObject<UGSS_GameSettingValueDiscreteDynamic_Bool>();
	TestTrue(TEXT("Bool options accept ExportText-style True"), BoolSetting->HasDynamicOption(TEXT("True")));
	TestTrue(TEXT("Bool options accept lowercase true"), BoolSetting->HasDynamicOption(TEXT("true")));
	TestTrue(TEXT("Bool options accept 1 as true"), BoolSetting->HasDynamicOption(TEXT("1")));

	BoolSetting->SetValueFromString(TEXT("True"));
	TestEqual(TEXT("Pending bool values store the canonical option spelling"), BoolSetting->GetValueAsString(), FString(TEXT("true")));
	TestEqual(TEXT("True maps to the ON option index"), BoolSetting->GetDiscreteOptionIndex(), 1);

	UGSS_GameSettingValueDiscreteDynamic* WindowMode = NewObject<UGSS_GameSettingValueDiscreteDynamic>();
	WindowMode->AddDynamicOption(TEXT("Fullscreen"), FText::FromString(TEXT("Fullscreen")));
	WindowMode->AddDynamicOption(TEXT("WindowedFullscreen"), FText::FromString(TEXT("Windowed Fullscreen")));
	WindowMode->AddDynamicOption(TEXT("Windowed"), FText::FromString(TEXT("Windowed")));
	WindowMode->SetDefaultValueFromString(TEXT("WindowedFullscreen"));
	WindowMode->SetValueFromString(TEXT("windowedfullscreen"));
	TestEqual(TEXT("Window mode stores the authored enumerator name"), WindowMode->GetValueAsString(), FString(TEXT("WindowedFullscreen")));
	TestEqual(TEXT("Windowed fullscreen maps to option index 1"), WindowMode->GetDiscreteOptionIndex(), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsLocalAccessorRoundTripTest, "GSS.Settings.Accessor.LocalGameUserSettings", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsLocalAccessorRoundTripTest::RunTest(const FString& Parameters)
{
	if (!GEngine || !GEngine->GetGameUserSettings())
	{
		AddWarning(TEXT("Skipped because UGameUserSettings is not available."));
		return true;
	}

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	const bool bOriginalVSync = Settings->IsVSyncEnabled();
	const EWindowMode::Type OriginalWindowMode = Settings->GetFullscreenMode();

	FGSS_SettingValueAccessor BoolAccessor;
	BoolAccessor.Source = EGSS_SettingValueSource::Local;
	BoolAccessor.GetterFunction = TEXT("IsVSyncEnabled");
	BoolAccessor.SetterFunction = TEXT("SetVSyncEnabled");

	FString BoolValue;
	TestTrue(TEXT("Local bool Getter can be read"), BoolAccessor.GetValue(nullptr, BoolValue));
	TestTrue(TEXT("Local bool Getter uses lowercase true/false"), BoolValue.Equals(TEXT("true"), ESearchCase::IgnoreCase) || BoolValue.Equals(TEXT("false"), ESearchCase::IgnoreCase));
	TestTrue(TEXT("Local bool Getter does not use ExportText True/False casing as the stored token"), BoolValue == TEXT("true") || BoolValue == TEXT("false"));

	FGSS_SettingValueAccessor EnumAccessor;
	EnumAccessor.Source = EGSS_SettingValueSource::Local;
	EnumAccessor.GetterFunction = TEXT("GetFullscreenMode");
	EnumAccessor.SetterFunction = TEXT("SetFullscreenMode");

	FString WindowModeValue;
	TestTrue(TEXT("Local enum Getter can be read"), EnumAccessor.GetValue(nullptr, WindowModeValue));
	TestTrue(TEXT("Local enum Getter uses enumerator names"),
		WindowModeValue == TEXT("Fullscreen") || WindowModeValue == TEXT("WindowedFullscreen") || WindowModeValue == TEXT("Windowed"));

	TestTrue(TEXT("Local enum Setter accepts enumerator names"), EnumAccessor.SetValue(nullptr, TEXT("Windowed")));
	FString WrittenWindowMode;
	TestTrue(TEXT("Local enum value can be read back"), EnumAccessor.GetValue(nullptr, WrittenWindowMode));
	TestEqual(TEXT("Local enum round-trip keeps the enumerator name"), WrittenWindowMode, FString(TEXT("Windowed")));

	Settings->SetVSyncEnabled(bOriginalVSync);
	Settings->SetFullscreenMode(OriginalWindowMode);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGSS_SettingsScalarDisplayFormatTest, "GSS.Settings.Scalar.DisplayFormat", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGSS_SettingsScalarDisplayFormatTest::RunTest(const FString& Parameters)
{
	UGSS_GameSettingRegistry* Registry = NewObject<UGSS_GameSettingRegistry>();
	UGSS_GameSettingsBuilder* Builder = NewObject<UGSS_GameSettingsBuilder>();
	Builder->Initialize(nullptr, Registry);

	UGSS_GameSettingDefinition_Scalar* Definition = NewObject<UGSS_GameSettingDefinition_Scalar>();
	Definition->SettingId = TAG_GSS_Settings_Test_Scalar;
	Definition->DisplayName = FText::FromString(TEXT("Volume"));
	Definition->Description = FText::FromString(TEXT("Test scalar"));
	Definition->Accessor = GSS_MAKE_LOCAL_ACCESSOR(GetFrameRateLimit, SetFrameRateLimit);
	Definition->DefaultValue = 0.5;
	Definition->MinimumValue = 0.0;
	Definition->MaximumValue = 1.0;
	Definition->Step = 0.01;
	Definition->DisplayFormat = EGSS_SettingScalarDisplayFormat::ZeroToOnePercent;

	UGSS_GameSettingValueScalarDynamic* Setting = Cast<UGSS_GameSettingValueScalarDynamic>(Builder->AddDefinition(Definition));
	TestNotNull(TEXT("Data Asset Scalar definitions create a runtime scalar node"), Setting);
	if (!Setting)
	{
		return true;
	}

	Setting->SetValue(0.5);
	TestEqual(TEXT("Normalized percent format is applied from the Data Asset definition"), Setting->GetFormattedText().ToString(), FString(TEXT("50%")));

	Setting->SetDisplayFormat(UGSS_GameSettingValueScalarDynamic::GetBuiltInDisplayFormat(EGSS_SettingScalarDisplayFormat::Raw));
	TestTrue(TEXT("Raw format no longer uses the percent formatter"), !Setting->GetFormattedText().ToString().Contains(TEXT("%")));
	return true;
}

#endif
