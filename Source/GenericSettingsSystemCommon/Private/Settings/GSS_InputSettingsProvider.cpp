// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_InputSettingsProvider.h"

#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_InputSettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_InputSettingsProvider"

FGSS_SettingValueAccessor UGSS_InputSettingsProvider::MakeSharedAccessor(FName Getter, FName Setter)
{
	FGSS_SettingValueAccessor Accessor;
	Accessor.Source = EGSS_SettingValueSource::Shared;
	Accessor.GetterFunction = Getter;
	Accessor.SetterFunction = Setter;
	return Accessor;
}

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input, "GSS.Settings.Input");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_MouseAndKeyboard, "GSS.Settings.Input.MouseAndKeyboard");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_MouseAndKeyboard_HorizontalSensitivity, "GSS.Settings.Input.MouseAndKeyboard.HorizontalSensitivity");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_MouseAndKeyboard_VerticalSensitivity, "GSS.Settings.Input.MouseAndKeyboard.VerticalSensitivity");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertVerticalAxis, "GSS.Settings.Input.MouseAndKeyboard.InvertVerticalAxis");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertHorizontalAxis, "GSS.Settings.Input.MouseAndKeyboard.InvertHorizontalAxis");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad, "GSS.Settings.Input.Gamepad");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_Hardware, "GSS.Settings.Input.Gamepad.Hardware");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_Vibration, "GSS.Settings.Input.Gamepad.Hardware.Vibration");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_InvertVerticalAxis, "GSS.Settings.Input.Gamepad.Hardware.InvertVerticalAxis");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_InvertHorizontalAxis, "GSS.Settings.Input.Gamepad.Hardware.InvertHorizontalAxis");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_Sensitivity, "GSS.Settings.Input.Gamepad.Sensitivity");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_LookSensitivity, "GSS.Settings.Input.Gamepad.Sensitivity.Look");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_AimSensitivity, "GSS.Settings.Input.Gamepad.Sensitivity.Aim");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_DeadZone, "GSS.Settings.Input.Gamepad.DeadZone");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_MoveStickDeadZone, "GSS.Settings.Input.Gamepad.DeadZone.MoveStick");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Input_Gamepad_LookStickDeadZone, "GSS.Settings.Input.Gamepad.DeadZone.LookStick");

void UGSS_InputSettingsProvider::RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Input, LOCTEXT("Input", "Input"), LOCTEXT("InputDescription", "Configure mouse, keyboard and gamepad preferences."));
	UGSS_GameSetting* MouseAndKeyboard = Builder->AddCollection(TAG_GSS_Settings_Input_MouseAndKeyboard, LOCTEXT("MouseAndKeyboard", "Mouse & Keyboard"), FText::GetEmpty(), Screen);
	Builder->AddScalar(TAG_GSS_Settings_Input_MouseAndKeyboard_HorizontalSensitivity, LOCTEXT("MouseHorizontalSensitivity", "Horizontal Sensitivity"), LOCTEXT("MouseHorizontalSensitivityDescription", "Control horizontal mouse look sensitivity."), 1.0, 0.1, 10.0, 0.1, MakeSharedAccessor(TEXT("GetMouseHorizontalSensitivity"), TEXT("SetMouseHorizontalSensitivity")), MouseAndKeyboard);
	Builder->AddScalar(TAG_GSS_Settings_Input_MouseAndKeyboard_VerticalSensitivity, LOCTEXT("MouseVerticalSensitivity", "Vertical Sensitivity"), LOCTEXT("MouseVerticalSensitivityDescription", "Control vertical mouse look sensitivity."), 1.0, 0.1, 10.0, 0.1, MakeSharedAccessor(TEXT("GetMouseVerticalSensitivity"), TEXT("SetMouseVerticalSensitivity")), MouseAndKeyboard);
	Builder->AddBool(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertVerticalAxis, LOCTEXT("MouseInvertVertical", "Invert Vertical Axis"), LOCTEXT("MouseInvertVerticalDescription", "Invert vertical mouse look."), false, MakeSharedAccessor(TEXT("GetInvertMouseVerticalAxis"), TEXT("SetInvertMouseVerticalAxis")), MouseAndKeyboard);
	Builder->AddBool(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertHorizontalAxis, LOCTEXT("MouseInvertHorizontal", "Invert Horizontal Axis"), LOCTEXT("MouseInvertHorizontalDescription", "Invert horizontal mouse look."), false, MakeSharedAccessor(TEXT("GetInvertMouseHorizontalAxis"), TEXT("SetInvertMouseHorizontalAxis")), MouseAndKeyboard);

	UGSS_GameSetting* Gamepad = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad, LOCTEXT("Gamepad", "Gamepad"), FText::GetEmpty(), Screen);
	UGSS_GameSetting* Hardware = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_Hardware, LOCTEXT("GamepadHardware", "Hardware"), FText::GetEmpty(), Gamepad);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_Vibration, LOCTEXT("GamepadVibration", "Vibration"), LOCTEXT("GamepadVibrationDescription", "Enable controller vibration feedback."), true, MakeSharedAccessor(TEXT("GetGamepadVibration"), TEXT("SetGamepadVibration")), Hardware);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_InvertVerticalAxis, LOCTEXT("GamepadInvertVertical", "Invert Vertical Axis"), LOCTEXT("GamepadInvertVerticalDescription", "Invert vertical gamepad look."), false, MakeSharedAccessor(TEXT("GetInvertGamepadVerticalAxis"), TEXT("SetInvertGamepadVerticalAxis")), Hardware);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_InvertHorizontalAxis, LOCTEXT("GamepadInvertHorizontal", "Invert Horizontal Axis"), LOCTEXT("GamepadInvertHorizontalDescription", "Invert horizontal gamepad look."), false, MakeSharedAccessor(TEXT("GetInvertGamepadHorizontalAxis"), TEXT("SetInvertGamepadHorizontalAxis")), Hardware);
	UGSS_GameSetting* Sensitivity = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_Sensitivity, LOCTEXT("GamepadSensitivity", "Sensitivity"), FText::GetEmpty(), Gamepad);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_LookSensitivity, LOCTEXT("GamepadLookSensitivity", "Look Sensitivity"), LOCTEXT("GamepadLookSensitivityDescription", "Control normal gamepad look sensitivity."), 1.0, 0.1, 10.0, 0.1, MakeSharedAccessor(TEXT("GetGamepadLookSensitivity"), TEXT("SetGamepadLookSensitivity")), Sensitivity);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_AimSensitivity, LOCTEXT("GamepadAimSensitivity", "Aim Sensitivity"), LOCTEXT("GamepadAimSensitivityDescription", "Control aimed gamepad look sensitivity."), 1.0, 0.1, 10.0, 0.1, MakeSharedAccessor(TEXT("GetGamepadAimSensitivity"), TEXT("SetGamepadAimSensitivity")), Sensitivity);
	UGSS_GameSetting* DeadZone = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_DeadZone, LOCTEXT("GamepadDeadZone", "Controller Dead Zone"), FText::GetEmpty(), Gamepad);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_MoveStickDeadZone, LOCTEXT("MoveStickDeadZone", "Left Stick Dead Zone"), LOCTEXT("MoveStickDeadZoneDescription", "Set the ignored range for the movement stick."), 0.1, 0.05, 0.95, 0.01, MakeSharedAccessor(TEXT("GetMoveStickDeadZone"), TEXT("SetMoveStickDeadZone")), DeadZone);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_LookStickDeadZone, LOCTEXT("LookStickDeadZone", "Right Stick Dead Zone"), LOCTEXT("LookStickDeadZoneDescription", "Set the ignored range for the look stick."), 0.1, 0.05, 0.95, 0.01, MakeSharedAccessor(TEXT("GetLookStickDeadZone"), TEXT("SetLookStickDeadZone")), DeadZone);
}

#undef LOCTEXT_NAMESPACE
