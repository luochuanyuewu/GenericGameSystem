// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Components/SlateWrapperTypes.h"
#include "GSS_GameSettingFilterState.h"
#include "GameplayTagContainer.h"

#include "GSS_GameSetting.generated.h"

class ULocalPlayer;
class UGSS_GameSettingRegistry;

//--------------------------------------
// UGSS_GameSetting
//--------------------------------------

DECLARE_DELEGATE_RetVal_OneParam(FText, FGetGameSettingsDetails, ULocalPlayer& /*InLocalPlayer*/);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class GENERICSETTINGSSYSTEM_API UGSS_GameSetting : public UObject
{
	GENERATED_BODY()

public:
	UGSS_GameSetting()
	{
	}

public:
	DECLARE_EVENT_TwoParams(UGSS_GameSetting, FOnSettingChanged, UGSS_GameSetting* /*InSetting*/, EGSS_GameSettingChangeReason /*InChangeReason*/);

	DECLARE_EVENT_OneParam(UGSS_GameSetting, FOnSettingApplied, UGSS_GameSetting* /*InSetting*/);

	DECLARE_EVENT_OneParam(UGSS_GameSetting, FOnSettingEditConditionChanged, UGSS_GameSetting* /*InSetting*/);

	FOnSettingChanged OnSettingChangedEvent;
	FOnSettingApplied OnSettingAppliedEvent;
	FOnSettingEditConditionChanged OnSettingEditConditionChangedEvent;

public:
	/**
	 * Gets the non-localized developer name for this setting.  This should remain constant, and represent a 
	 * unique identifier for this setting inside this settings registry.
	 */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	FName GetDevName() const { return DevName; }

	void SetDevName(const FName& Value) { DevName = Value; }

	bool GetAdjustListViewPostRefresh() const { return bAdjustListViewPostRefresh; }
	void SetAdjustListViewPostRefresh(const bool Value) { bAdjustListViewPostRefresh = Value; }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	FText GetDisplayName() const { return DisplayName; }

	void SetDisplayName(const FText& Value) { DisplayName = Value; }
#if !UE_BUILD_SHIPPING
	void SetDisplayName(const FString& Value) { SetDisplayName(FText::FromString(Value)); }
#endif
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	ESlateVisibility GetDisplayNameVisibility() { return DisplayNameVisibility; }

	void SetNameDisplayVisibility(ESlateVisibility InVisibility) { DisplayNameVisibility = InVisibility; }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	FText GetDescriptionRichText() const { return DescriptionRichText; }

	void SetDescriptionRichText(const FText& Value)
	{
		DescriptionRichText = Value;
		InvalidateSearchableText();
	}

	/** This version is for cheats and other non-shipping items, that don't need to localize their text.  We don't permit this in shipping to prevent unlocalized text being introduced. */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	void SetDescriptionRichText(const FString& Value) { SetDescriptionRichText(FText::FromString(Value)); }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	const FGameplayTagContainer& GetTags() const { return Tags; }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	void AddTag(const FGameplayTag& TagToAdd) { Tags.AddTag(TagToAdd); }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	void SetRegistry(UGSS_GameSettingRegistry* InOwningRegistry) { OwningRegistry = InOwningRegistry; }

	/** Gets the searchable plain text for the description. */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	const FString& GetDescriptionPlainText() const;

	/** Initializes the setting, giving it the owning local player.  Containers automatically initialize settings added to them. */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	void Initialize(ULocalPlayer* InLocalPlayer);

	/** Gets the owning local player for this setting - which all initialized settings will have. */
	UFUNCTION(BlueprintPure, Category = "GGF|GameSettings")
	ULocalPlayer* GetOwningLocalPlayer() const { return LocalPlayer; }

	/** Set the dynamic details callback, we query this when building the description panel.  This text is not searchable.*/
	void SetDynamicDetails(const FGetGameSettingsDetails& InDynamicDetails) { DynamicDetails = InDynamicDetails; }

	/**
	 * Gets the dynamic details about this setting.  This may be information like, how many refunds are remaining 
	 * on their account, or the account number.
	 */
	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	FText GetDynamicDetails() const;

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	FText GetWarningRichText() const { return WarningRichText; }

	UFUNCTION(BlueprintCallable, Category = "GGF|GameSettings")
	void SetWarningRichText(const FText& Value)
	{
		WarningRichText = Value;
		InvalidateSearchableText();
	}

	/** This version is for cheats and other non-shipping items, that don't need to localize their text.  We don't permit this in shipping to prevent unlocalized text being introduced. */
	void SetWarningRichText(const FString& Value) { SetWarningRichText(FText::FromString(Value)); }

	/**
	 * Gets the edit state of this property based on the current state of its edit conditions as well as any additional
	 * filter state.
	 */
	const FGSS_GameSettingEditableState& GetEditState() const { return EditableStateCache; }

	/** Adds a new edit condition to this setting, allowing you to control the visibility and edit-ability of this setting. */
	void AddEditCondition(const TSharedRef<FGSS_GameSettingEditCondition>& InEditCondition);

	/** Add setting dependency, if these settings change, we'll re-evaluate edit conditions for this setting. */
	void AddEditDependency(UGSS_GameSetting* DependencySetting);

	/** The parent object that owns the setting, in most cases the collection, but for top level settings the registry. */
	void SetSettingParent(UGSS_GameSetting* InSettingParent);
	UGSS_GameSetting* GetSettingParent() const { return SettingParent; }

	/** Should this setting be reported to analytics. */
	bool GetIsReportedToAnalytics() const { return bReportAnalytics; }
	void SetIsReportedToAnalytics(bool bReport) { bReportAnalytics = bReport; }

	/** Gets the analytics value for this setting. */
	virtual FString GetAnalyticsValue() const { return TEXT(""); }

	/**
	 * Some settings may take an async amount of time to finish initializing.  The settings system will wait
	 * for all settings to be ready before showing the setting.
	 */
	bool IsReady() const { return bReady; }

	/**
	 * Any setting can have children, this is so we can allow for the possibility of "collections" or "actions" that
	 * are not directly visible to the user, but are set by some means and need to have initial and restored values.
	 * In that case, you would likely have internal settings inside an action subclass that is set on another screen,
	 * but never directly listed on the settings panel.
	 */
	virtual TArray<UGSS_GameSetting*> GetChildSettings() { return TArray<UGSS_GameSetting*>(); }

	/**
	 * Refresh the editable state of the setting and notify that the state has changed so that any UI currently
	 * examining this setting is updated with the new options, or whatever.
	 */
	void RefreshEditableState(bool bNotifyEditConditionsChanged = true);

	/**
	 * We expect settings to change the live value immediately, but occasionally there are special settings
	 * that go are immediately stored to a temporary location but we don't actually apply them until later
	 * like selecting a new resolution.
	 */
	void Apply();

	/** Gets the current world of the local player that owns these settings. */
	virtual UWorld* GetWorld() const override;

protected:
	/**  */
	virtual void Startup();
	void StartupComplete();

	virtual void OnInitialized();
	virtual void OnApply();
	virtual void OnGatherEditState(FGSS_GameSettingEditableState& InOutEditState) const;
	virtual void OnDependencyChanged();

	/**  */
	virtual FText GetDynamicDetailsInternal() const;

	/** */
	void HandleEditDependencyChanged(UGSS_GameSetting* DependencySetting, EGSS_GameSettingChangeReason Reason);
	void HandleEditDependencyChanged(UGSS_GameSetting* DependencySetting);

	/** Regenerates the plain searchable text if it has been dirtied. */
	void RefreshPlainText() const;
	void InvalidateSearchableText() { bRefreshPlainSearchableText = true; }

	/** Notify that the setting changed */
	void NotifySettingChanged(EGSS_GameSettingChangeReason Reason);
	virtual void OnSettingChanged(EGSS_GameSettingChangeReason Reason);

	/** Notify that the settings edit conditions changed.  This may mean it's now invisible, or disabled, or possibly that the options have changed in some meaningful way. */
	void NotifyEditConditionsChanged();
	virtual void OnEditConditionsChanged();

	/**  */
	FGSS_GameSettingEditableState ComputeEditableState() const;

protected:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSetting> SettingParent;

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingRegistry> OwningRegistry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GGF|GameSettings")
	FName DevName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GGF|GameSettings")
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GGF|GameSettings")
	ESlateVisibility DisplayNameVisibility = ESlateVisibility::SelfHitTestInvisible;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GGF|GameSettings")
	FText DescriptionRichText;
	FText WarningRichText;

	/** A collection of tags for the settings.  These can just be arbitrary flags used by the UI to do different things. */
	FGameplayTagContainer Tags;

	FGetGameSettingsDetails DynamicDetails;

	/** Any edit conditions for this setting. */
	TArray<TSharedRef<FGSS_GameSettingEditCondition>> EditConditions;

	class FStringCultureCache
	{
		FStringCultureCache(TFunction<FString()> InStringGetter);

		void Invalidate();

		FString Get() const;

	private:
		mutable FString StringCache;
		mutable FCultureRef Culture;
		TFunction<FString()> StringGetter;
	};

	/** When the text changes, we invalidate the searchable text. */
	mutable bool bRefreshPlainSearchableText = true;
	/** When we set the rich text for a setting, we automatically generate the plain text. */
	mutable FString AutoGenerated_DescriptionPlainText;

	/** Report as part of analytics, by default no setting reports, except GSS_GameSettingValues. */
	bool bReportAnalytics = false;

private:
	/** Most settings are immediately ready, but some may require startup time before it's safe to call their functions. */
	bool bReady = false;

	/** Prevent re-entrancy problems when announcing a setting has changed. */
	bool bOnSettingChangedEventGuard = false;

	/** Prevent re-entrancy problems when announcing a setting has changed edit conditions. */
	bool bOnEditConditionsChangedEventGuard = false;

	/**  */
	bool bAdjustListViewPostRefresh = true;

	/** We cache the editable state of a setting when it changes rather than reprocessing it any time it's needed.  */
	FGSS_GameSettingEditableState EditableStateCache;
};
