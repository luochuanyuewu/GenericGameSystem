// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider_Audio.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_CommonSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider_Audio)

#define LOCTEXT_NAMESPACE "GSS_GameSettingsProvider_Audio"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio, "GSS.Settings.Audio");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume, "GSS.Settings.Audio.Volume");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Sound, "GSS.Settings.Audio.Sound");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Master, "GSS.Settings.Audio.Volume.Master");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Music, "GSS.Settings.Audio.Volume.Music");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_SoundEffects, "GSS.Settings.Audio.Volume.SoundEffects");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Dialogue, "GSS.Settings.Audio.Volume.Dialogue");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_VoiceChat, "GSS.Settings.Audio.Volume.VoiceChat");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Sound_MuteAll, "GSS.Settings.Audio.Sound.MuteAll");

UGSS_GameSettingsProvider_Audio::UGSS_GameSettingsProvider_Audio()
{
	AudioText = LOCTEXT("Audio", "Audio");
	AudioDescriptionText = LOCTEXT("AudioDescription", "Configure audio preferences.");
	VolumeText = LOCTEXT("Volume", "Volume");
	MasterVolumeText = LOCTEXT("MasterVolume", "Master Volume");
	MasterVolumeDescriptionText = LOCTEXT("MasterVolumeDescription", "Control the overall game volume.");
	MusicVolumeText = LOCTEXT("MusicVolume", "Music Volume");
	MusicVolumeDescriptionText = LOCTEXT("MusicVolumeDescription", "Control music volume.");
	SFXVolumeText = LOCTEXT("SFXVolume", "Sound Effects Volume");
	SFXVolumeDescriptionText = LOCTEXT("SFXVolumeDescription", "Control sound effects volume.");
	DialogueVolumeText = LOCTEXT("DialogueVolume", "Dialogue Volume");
	DialogueVolumeDescriptionText = LOCTEXT("DialogueVolumeDescription", "Control character dialogue and voice-over volume.");
	VoiceChatVolumeText = LOCTEXT("VoiceChatVolume", "Voice Chat Volume");
	VoiceChatVolumeDescriptionText = LOCTEXT("VoiceChatVolumeDescription", "Control multiplayer voice chat volume.");
	SoundText = LOCTEXT("Sound", "Sound");
	MuteAllText = LOCTEXT("MuteAll", "Mute All Audio");
	MuteAllDescriptionText = LOCTEXT("MuteAllDescription", "Mute all game audio.");
}

void UGSS_GameSettingsProvider_Audio::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Audio, AudioText, AudioDescriptionText);
	UGSS_GameSetting* VolumeCollection = Builder->AddCollection(TAG_GSS_Settings_Audio_Volume, VolumeText, FText::GetEmpty(), Screen);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Master, MasterVolumeText, MasterVolumeDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMasterVolume, SetMasterVolume), VolumeCollection);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Music, MusicVolumeText, MusicVolumeDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMusicVolume, SetMusicVolume), VolumeCollection);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_SoundEffects, SFXVolumeText, SFXVolumeDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSoundEffectsVolume, SetSoundEffectsVolume), VolumeCollection);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Dialogue, DialogueVolumeText, DialogueVolumeDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetDialogueVolume, SetDialogueVolume), VolumeCollection);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_VoiceChat, VoiceChatVolumeText, VoiceChatVolumeDescriptionText, 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetVoiceChatVolume, SetVoiceChatVolume), VolumeCollection);

	UGSS_GameSetting* SoundCollection = Builder->AddCollection(TAG_GSS_Settings_Audio_Sound, SoundText, FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Audio_Sound_MuteAll, MuteAllText, MuteAllDescriptionText, false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMuteAllAudio, SetMuteAllAudio), SoundCollection);
}

#undef LOCTEXT_NAMESPACE
