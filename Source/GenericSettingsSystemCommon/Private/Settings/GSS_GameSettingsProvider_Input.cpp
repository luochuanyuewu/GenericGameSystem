// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider_Input.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_CommonSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider_Input)

#define LOCTEXT_NAMESPACE "GSS_GameSettingsProvider_Input"

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

UGSS_GameSettingsProvider_Input::UGSS_GameSettingsProvider_Input()
{
	InputText = LOCTEXT("Input", "Input");
	InputDescriptionText = LOCTEXT("InputDescription", "Configure mouse, keyboard and gamepad preferences.");
	MouseAndKeyboardText = LOCTEXT("MouseAndKeyboard", "Mouse & Keyboard");
	MouseHorizontalSensitivityText = LOCTEXT("MouseHorizontalSensitivity", "Horizontal Sensitivity");
	MouseHorizontalSensitivityDescriptionText = LOCTEXT("MouseHorizontalSensitivityDescription", "Control horizontal mouse look sensitivity.");
	MouseVerticalSensitivityText = LOCTEXT("MouseVerticalSensitivity", "Vertical Sensitivity");
	MouseVerticalSensitivityDescriptionText = LOCTEXT("MouseVerticalSensitivityDescription", "Control vertical mouse look sensitivity.");
	MouseInvertVerticalText = LOCTEXT("MouseInvertVertical", "Invert Vertical Axis");
	MouseInvertVerticalDescriptionText = LOCTEXT("MouseInvertVerticalDescription", "Invert vertical mouse look.");
	MouseInvertHorizontalText = LOCTEXT("MouseInvertHorizontal", "Invert Horizontal Axis");
	MouseInvertHorizontalDescriptionText = LOCTEXT("MouseInvertHorizontalDescription", "Invert horizontal mouse look.");
	GamepadText = LOCTEXT("Gamepad", "Gamepad");
	GamepadHardwareText = LOCTEXT("GamepadHardware", "Hardware");
	GamepadVibrationText = LOCTEXT("GamepadVibration", "Vibration");
	GamepadVibrationDescriptionText = LOCTEXT("GamepadVibrationDescription", "Enable controller vibration feedback.");
	GamepadInvertVerticalText = LOCTEXT("GamepadInvertVertical", "Invert Vertical Axis");
	GamepadInvertVerticalDescriptionText = LOCTEXT("GamepadInvertVerticalDescription", "Invert vertical gamepad look.");
	GamepadInvertHorizontalText = LOCTEXT("GamepadInvertHorizontal", "Invert Horizontal Axis");
	GamepadInvertHorizontalDescriptionText = LOCTEXT("GamepadInvertHorizontalDescription", "Invert horizontal gamepad look.");
	GamepadSensitivityText = LOCTEXT("GamepadSensitivity", "Sensitivity");
	GamepadLookSensitivityText = LOCTEXT("GamepadLookSensitivity", "Look Sensitivity");
	GamepadLookSensitivityDescriptionText = LOCTEXT("GamepadLookSensitivityDescription", "Control normal gamepad look sensitivity.");
	GamepadAimSensitivityText = LOCTEXT("GamepadAimSensitivity", "Aim Sensitivity");
	GamepadAimSensitivityDescriptionText = LOCTEXT("GamepadAimSensitivityDescription", "Control aimed gamepad look sensitivity.");
	GamepadDeadZoneText = LOCTEXT("GamepadDeadZone", "Controller Dead Zone");
	MoveStickDeadZoneText = LOCTEXT("MoveStickDeadZone", "Left Stick Dead Zone");
	MoveStickDeadZoneDescriptionText = LOCTEXT("MoveStickDeadZoneDescription", "Set the ignored range for the movement stick.");
	LookStickDeadZoneText = LOCTEXT("LookStickDeadZone", "Right Stick Dead Zone");
	LookStickDeadZoneDescriptionText = LOCTEXT("LookStickDeadZoneDescription", "Set the ignored range for the look stick.");
}

void UGSS_GameSettingsProvider_Input::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Input, InputText, InputDescriptionText);
	UGSS_GameSetting* MouseAndKeyboardCollection = Builder->AddCollection(TAG_GSS_Settings_Input_MouseAndKeyboard, MouseAndKeyboardText, FText::GetEmpty(), Screen);
	Builder->AddScalar(TAG_GSS_Settings_Input_MouseAndKeyboard_HorizontalSensitivity, MouseHorizontalSensitivityText, MouseHorizontalSensitivityDescriptionText, 1.0, 0.1, 10.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMouseHorizontalSensitivity, SetMouseHorizontalSensitivity), MouseAndKeyboardCollection);
	Builder->AddScalar(TAG_GSS_Settings_Input_MouseAndKeyboard_VerticalSensitivity, MouseVerticalSensitivityText, MouseVerticalSensitivityDescriptionText, 1.0, 0.1, 10.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMouseVerticalSensitivity, SetMouseVerticalSensitivity), MouseAndKeyboardCollection);
	Builder->AddBool(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertVerticalAxis, MouseInvertVerticalText, MouseInvertVerticalDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetInvertMouseVerticalAxis, SetInvertMouseVerticalAxis), MouseAndKeyboardCollection);
	Builder->AddBool(TAG_GSS_Settings_Input_MouseAndKeyboard_InvertHorizontalAxis, MouseInvertHorizontalText, MouseInvertHorizontalDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetInvertMouseHorizontalAxis, SetInvertMouseHorizontalAxis), MouseAndKeyboardCollection);

	UGSS_GameSetting* GamepadCollection = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad, GamepadText, FText::GetEmpty(), Screen);
	UGSS_GameSetting* Hardware = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_Hardware, GamepadHardwareText, FText::GetEmpty(), GamepadCollection);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_Vibration, GamepadVibrationText, GamepadVibrationDescriptionText, true, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetGamepadVibration, SetGamepadVibration), Hardware);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_InvertVerticalAxis, GamepadInvertVerticalText, GamepadInvertVerticalDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetInvertGamepadVerticalAxis, SetInvertGamepadVerticalAxis), Hardware);
	Builder->AddBool(TAG_GSS_Settings_Input_Gamepad_InvertHorizontalAxis, GamepadInvertHorizontalText, GamepadInvertHorizontalDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetInvertGamepadHorizontalAxis, SetInvertGamepadHorizontalAxis), Hardware);
	UGSS_GameSetting* Sensitivity = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_Sensitivity, GamepadSensitivityText, FText::GetEmpty(), GamepadCollection);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_LookSensitivity, GamepadLookSensitivityText, GamepadLookSensitivityDescriptionText, 1.0, 0.1, 10.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetGamepadLookSensitivity, SetGamepadLookSensitivity), Sensitivity);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_AimSensitivity, GamepadAimSensitivityText, GamepadAimSensitivityDescriptionText, 1.0, 0.1, 10.0, 0.1, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetGamepadAimSensitivity, SetGamepadAimSensitivity), Sensitivity);
	UGSS_GameSetting* DeadZone = Builder->AddCollection(TAG_GSS_Settings_Input_Gamepad_DeadZone, GamepadDeadZoneText, FText::GetEmpty(), GamepadCollection);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_MoveStickDeadZone, MoveStickDeadZoneText, MoveStickDeadZoneDescriptionText, 0.1, 0.05, 0.95, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMoveStickDeadZone, SetMoveStickDeadZone), DeadZone);
	Builder->AddScalar(TAG_GSS_Settings_Input_Gamepad_LookStickDeadZone, LookStickDeadZoneText, LookStickDeadZoneDescriptionText, 0.1, 0.05, 0.95, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetLookStickDeadZone, SetLookStickDeadZone), DeadZone);
}

#undef LOCTEXT_NAMESPACE
