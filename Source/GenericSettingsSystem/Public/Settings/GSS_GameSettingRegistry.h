// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"
#include "Templates/Casts.h"

#include "GSS_GameSettingRegistry.generated.h"

struct FGameplayTag;

//--------------------------------------
// UGSS_GameSettingRegistry
//--------------------------------------

class ULocalPlayer;
struct FGSS_GameSettingFilterState;

enum class EGSS_GameSettingChangeReason : uint8;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingRegistry : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_TwoParams(UGSS_GameSettingRegistry, FOnSettingChanged, UGSS_GameSetting*, EGSS_GameSettingChangeReason);

	DECLARE_EVENT_OneParam(UGSS_GameSettingRegistry, FOnSettingEditConditionChanged, UGSS_GameSetting*);

	FOnSettingChanged OnSettingChangedEvent;
	FOnSettingEditConditionChanged OnSettingEditConditionChangedEvent;

	DECLARE_EVENT_TwoParams(UGSS_GameSettingRegistry, FOnSettingNamedActionEvent, UGSS_GameSetting* /*Setting*/, FGameplayTag /*GameSettings_Action_Tag*/);

	FOnSettingNamedActionEvent OnSettingNamedActionEvent;

	/** Navigate to the child settings of the provided setting. */
	DECLARE_EVENT_OneParam(UGSS_GameSettingRegistry, FOnExecuteNavigation, UGSS_GameSetting* /*Setting*/);

	FOnExecuteNavigation OnExecuteNavigationEvent;

public:
	UGSS_GameSettingRegistry();

	void Initialize(ULocalPlayer* InLocalPlayer);

	virtual void Regenerate();

	virtual bool IsFinishedInitializing() const;

	virtual void SaveChanges();

	void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings);

	UGSS_GameSetting* FindSettingByDevName(const FName& SettingDevName);

	template <typename T = UGSS_GameSetting>
	T* FindSettingByDevNameChecked(const FName& SettingDevName)
	{
		T* Setting = Cast<T>(FindSettingByDevName(SettingDevName));
		check(Setting);
		return Setting;
	}

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) PURE_VIRTUAL(,)

	virtual void OnSettingApplied(UGSS_GameSetting* Setting)
	{
	}

	void RegisterSetting(UGSS_GameSetting* InSetting);
	void RegisterInnerSettings(UGSS_GameSetting* InSetting);

	// Internal event handlers.
	void HandleSettingChanged(UGSS_GameSetting* Setting, EGSS_GameSettingChangeReason Reason);
	void HandleSettingApplied(UGSS_GameSetting* Setting);
	void HandleSettingEditConditionsChanged(UGSS_GameSetting* Setting);
	void HandleSettingNamedAction(UGSS_GameSetting* Setting, FGameplayTag GameSettings_Action_Tag);
	void HandleSettingNavigation(UGSS_GameSetting* Setting);

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> TopLevelSettings;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> RegisteredSettings;

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> OwningLocalPlayer;
};
