// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_AudioSettingsProvider.h"

#include "NativeGameplayTags.h"
#include "Settings/GSS_CommonSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_AudioSettingsProvider)

#define LOCTEXT_NAMESPACE "GSS_AudioSettingsProvider"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio, "GSS.Settings.Audio");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume, "GSS.Settings.Audio.Volume");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Sound, "GSS.Settings.Audio.Sound");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Master, "GSS.Settings.Audio.Volume.Master");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Music, "GSS.Settings.Audio.Volume.Music");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_SoundEffects, "GSS.Settings.Audio.Volume.SoundEffects");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_Dialogue, "GSS.Settings.Audio.Volume.Dialogue");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Volume_VoiceChat, "GSS.Settings.Audio.Volume.VoiceChat");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GSS_Settings_Audio_Sound_MuteAll, "GSS.Settings.Audio.Sound.MuteAll");

void UGSS_AudioSettingsProvider::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
	if (!Builder)
	{
		return;
	}

	UGSS_GameSetting* Screen = Builder->AddCollection(TAG_GSS_Settings_Audio, LOCTEXT("Audio", "Audio"), LOCTEXT("AudioDescription", "Configure audio preferences."));
	UGSS_GameSetting* Volume = Builder->AddCollection(TAG_GSS_Settings_Audio_Volume, LOCTEXT("Volume", "Volume"), FText::GetEmpty(), Screen);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Master, LOCTEXT("MasterVolume", "Master Volume"), LOCTEXT("MasterVolumeDescription", "Control the overall game volume."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMasterVolume, SetMasterVolume), Volume);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Music, LOCTEXT("MusicVolume", "Music Volume"), LOCTEXT("MusicVolumeDescription", "Control music volume."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMusicVolume, SetMusicVolume), Volume);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_SoundEffects, LOCTEXT("SFXVolume", "Sound Effects Volume"), LOCTEXT("SFXVolumeDescription", "Control sound effects volume."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetSoundEffectsVolume, SetSoundEffectsVolume), Volume);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_Dialogue, LOCTEXT("DialogueVolume", "Dialogue Volume"), LOCTEXT("DialogueVolumeDescription", "Control character dialogue and voice-over volume."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetDialogueVolume, SetDialogueVolume), Volume);
	Builder->AddScalar(TAG_GSS_Settings_Audio_Volume_VoiceChat, LOCTEXT("VoiceChatVolume", "Voice Chat Volume"), LOCTEXT("VoiceChatVolumeDescription", "Control multiplayer voice chat volume."), 1.0, 0.0, 1.0, 0.01, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetVoiceChatVolume, SetVoiceChatVolume), Volume);

	UGSS_GameSetting* Sound = Builder->AddCollection(TAG_GSS_Settings_Audio_Sound, LOCTEXT("Sound", "Sound"), FText::GetEmpty(), Screen);
	Builder->AddBool(TAG_GSS_Settings_Audio_Sound_MuteAll, LOCTEXT("MuteAll", "Mute All Audio"), LOCTEXT("MuteAllDescription", "Mute all game audio."), false, GSS_MAKE_SHARED_ACCESSOR(UGSS_CommonSettingsShared, GetMuteAllAudio, SetMuteAllAudio), Sound);
}

#undef LOCTEXT_NAMESPACE
