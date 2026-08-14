// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"

#include "GSS_GameSettingCollection.generated.h"

struct FGSS_GameSettingFilterState;

//--------------------------------------
// UGSS_GameSettingCollection
//--------------------------------------

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingCollection : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingCollection();

	virtual TArray<UGSS_GameSetting*> GetChildSettings() override { return Settings; }
	TArray<UGSS_GameSettingCollection*> GetChildCollections() const;

	void AddSetting(UGSS_GameSetting* Setting);
	virtual void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const;

	virtual bool IsSelectable() const { return false; }

protected:
	/** The settings owned by this collection. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> Settings;

	UPROPERTY(EditAnywhere, Instanced, Category = "GGF|GameSettings")
	TArray<TObjectPtr<UGSS_GameSetting>> GameSettings;
};

//--------------------------------------
// UGSS_GameSettingCollectionPage
//--------------------------------------

UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingCollectionPage : public UGSS_GameSettingCollection
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UGSS_GameSettingCollectionPage, FOnExecuteNavigation, UGSS_GameSetting* /*Setting*/);

	FOnExecuteNavigation OnExecuteNavigationEvent;

public:
	UGSS_GameSettingCollectionPage();

	FText GetNavigationText() const { return NavigationText; }
	void SetNavigationText(FText Value) { NavigationText = Value; }
#if !UE_BUILD_SHIPPING
	void SetNavigationText(const FString& Value) { SetNavigationText(FText::FromString(Value)); }
#endif

	virtual void OnInitialized() override;
	virtual void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const override;
	virtual bool IsSelectable() const override { return true; }

	/**  */
	void ExecuteNavigation();

private:
	FText NavigationText;
};
