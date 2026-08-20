// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_GameSettingsValueEntries.h"

#include "AnalogSlider.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingValueDiscrete.h"
#include "Settings/GSS_GameSettingValueScalar.h"
#include "SettingsUI/GSS_GameSettingRotator.h"
#include "Components/PanelWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsValueEntries)

TArray<FText> UGSS_GameSettingListEntry_Discrete::GetOptions() const
{
	if (const UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) return Discrete->GetDiscreteOptions();
	return {};
}

void UGSS_GameSettingListEntry_Discrete::SetSetting(UGSS_GameSetting* InSetting)
{
	DiscreteSetting = Cast<UGSS_GameSettingValueDiscrete>(InSetting);
	Super::SetSetting(InSetting);
	RefreshDiscreteControl();
}

void UGSS_GameSettingListEntry_Discrete::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Rotator_SettingValue)
	{
		Rotator_SettingValue->OnRotatedEvent.AddUObject(this, &ThisClass::HandleRotatorChangedValue);
	}
	if (Button_Decrease)
	{
		Button_Decrease->OnClicked().AddUObject(this, &ThisClass::HandleOptionDecrease);
	}
	if (Button_Increase)
	{
		Button_Increase->OnClicked().AddUObject(this, &ThisClass::HandleOptionIncrease);
	}
}

void UGSS_GameSettingListEntry_Discrete::NativeOnEntryReleased()
{
	DiscreteSetting = nullptr;
	Super::NativeOnEntryReleased();
}
int32 UGSS_GameSettingListEntry_Discrete::GetSelectedOptionIndex() const
{
	if (const UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) return Discrete->GetDiscreteOptionIndex();
	return INDEX_NONE;
}
int32 UGSS_GameSettingListEntry_Discrete::GetDefaultOptionIndex() const
{
	if (const UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting)) return Discrete->GetDiscreteOptionDefaultIndex();
	return INDEX_NONE;
}
bool UGSS_GameSettingListEntry_Discrete::SelectOption(int32 OptionIndex)
{
	if (UGSS_GameSettingValueDiscrete* Discrete = Cast<UGSS_GameSettingValueDiscrete>(Setting); Discrete && CanInteractWithSetting() && Discrete->GetDiscreteOptions().IsValidIndex(OptionIndex)) { Discrete->SetDiscreteOptionByIndex(OptionIndex); return true; }
	return false;
}

void UGSS_GameSettingListEntry_Discrete::HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason)
{
	if (!bSuspendChangeUpdates)
	{
		Super::HandleSettingChanged(ChangedSetting, Reason);
		RefreshDiscreteControl();
	}
}

void UGSS_GameSettingListEntry_Discrete::HandleEditStateChanged(UGSS_GameSetting* ChangedSetting)
{
	DiscreteSetting = Cast<UGSS_GameSettingValueDiscrete>(Setting);
	Super::HandleEditStateChanged(ChangedSetting);
	RefreshDiscreteControl();
	const bool bEnabled = IsSettingEnabled();
	if (Panel_Value) Panel_Value->SetIsEnabled(bEnabled);
	if (Rotator_SettingValue) Rotator_SettingValue->SetIsEnabled(bEnabled);
	if (Button_Decrease) Button_Decrease->SetIsEnabled(bEnabled);
	if (Button_Increase) Button_Increase->SetIsEnabled(bEnabled);
}

void UGSS_GameSettingListEntry_Discrete::RefreshDiscreteControl()
{
	if (!DiscreteSetting || !Rotator_SettingValue)
	{
		return;
	}

	const TArray<FText> Options = DiscreteSetting->GetDiscreteOptions();
	Rotator_SettingValue->PopulateTextLabels(Options);
	Rotator_SettingValue->SetSelectedItem(DiscreteSetting->GetDiscreteOptionIndex());
	Rotator_SettingValue->SetDefaultOption(DiscreteSetting->GetDiscreteOptionDefaultIndex());
}

void UGSS_GameSettingListEntry_Discrete::HandleOptionDecrease()
{
	if (Rotator_SettingValue && CanInteractWithSetting())
	{
		Rotator_SettingValue->ShiftTextLeft();
		SelectOption(Rotator_SettingValue->GetSelectedIndex());
	}
}

void UGSS_GameSettingListEntry_Discrete::HandleOptionIncrease()
{
	if (Rotator_SettingValue && CanInteractWithSetting())
	{
		Rotator_SettingValue->ShiftTextRight();
		SelectOption(Rotator_SettingValue->GetSelectedIndex());
	}
}

void UGSS_GameSettingListEntry_Discrete::HandleRotatorChangedValue(int32 Value, bool bUserInitiated)
{
	if (bUserInitiated)
	{
		SelectOption(Value);
	}
}
double UGSS_GameSettingListEntry_Scalar::GetNormalizedValue() const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) return Scalar->GetValueNormalized();
	return 0.0;
}

void UGSS_GameSettingListEntry_Scalar::SetSetting(UGSS_GameSetting* InSetting)
{
	ScalarSetting = Cast<UGSS_GameSettingValueScalar>(InSetting);
	Super::SetSetting(InSetting);
	RefreshScalarControl();
}

void UGSS_GameSettingListEntry_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Slider_SettingValue)
	{
		Slider_SettingValue->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
		Slider_SettingValue->OnMouseCaptureEnd.AddDynamic(this, &ThisClass::HandleSliderCaptureEnded);
		Slider_SettingValue->OnControllerCaptureEnd.AddDynamic(this, &ThisClass::HandleSliderCaptureEnded);
	}
}

void UGSS_GameSettingListEntry_Scalar::NativeOnEntryReleased()
{
	ScalarSetting = nullptr;
	Super::NativeOnEntryReleased();
}
FText UGSS_GameSettingListEntry_Scalar::GetFormattedValue() const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) return Scalar->GetFormattedText();
	return FText::GetEmpty();
}
bool UGSS_GameSettingListEntry_Scalar::GetDefaultNormalizedValue(double& OutValue) const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting); Scalar && Scalar->GetDefaultValueNormalized().IsSet())
	{
		OutValue = Scalar->GetDefaultValueNormalized().GetValue();
		return true;
	}
	OutValue = 0.0;
	return false;
}
double UGSS_GameSettingListEntry_Scalar::GetNormalizedStepSize() const
{
	if (const UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting)) return Scalar->GetNormalizedStepSize();
	return 0.0;
}
bool UGSS_GameSettingListEntry_Scalar::SetNormalizedValue(double Value)
{
	if (UGSS_GameSettingValueScalar* Scalar = Cast<UGSS_GameSettingValueScalar>(Setting); Scalar && CanInteractWithSetting()) { Scalar->SetValueNormalized(Value); return true; }
	return false;
}

void UGSS_GameSettingListEntry_Scalar::HandleSettingChanged(UGSS_GameSetting* ChangedSetting, EGSS_GameSettingChangeReason Reason)
{
	if (!bSuspendChangeUpdates)
	{
		Super::HandleSettingChanged(ChangedSetting, Reason);
		RefreshScalarControl();
	}
}

void UGSS_GameSettingListEntry_Scalar::HandleEditStateChanged(UGSS_GameSetting* ChangedSetting)
{
	ScalarSetting = Cast<UGSS_GameSettingValueScalar>(Setting);
	Super::HandleEditStateChanged(ChangedSetting);
	RefreshScalarControl();
	const bool bEnabled = IsSettingEnabled();
	if (Panel_Value) Panel_Value->SetIsEnabled(bEnabled);
	if (Slider_SettingValue) Slider_SettingValue->SetIsEnabled(bEnabled);
}

void UGSS_GameSettingListEntry_Scalar::RefreshScalarControl()
{
	if (!ScalarSetting)
	{
		return;
	}

	const float NormalizedValue = static_cast<float>(ScalarSetting->GetValueNormalized());
	if (Slider_SettingValue)
	{
		Slider_SettingValue->SetValue(NormalizedValue);
		Slider_SettingValue->SetStepSize(static_cast<float>(ScalarSetting->GetNormalizedStepSize()));
	}
	if (Text_SettingValue)
	{
		Text_SettingValue->SetText(ScalarSetting->GetFormattedText());
	}
	const TOptional<double> DefaultValue = ScalarSetting->GetDefaultValueNormalized();
	if (DefaultValue.IsSet())
	{
		OnDefaultValueChanged(static_cast<float>(DefaultValue.GetValue()));
	}
	OnValueChanged(NormalizedValue);
}

void UGSS_GameSettingListEntry_Scalar::HandleSliderValueChanged(float Value)
{
	if (!CanInteractWithSetting())
	{
		return;
	}

	TGuardValue<bool> Guard(bSuspendChangeUpdates, true);
	if (SetNormalizedValue(Value))
	{
		RefreshScalarControl();
	}
}

void UGSS_GameSettingListEntry_Scalar::HandleSliderCaptureEnded()
{
}
FText UGSS_GameSettingListEntry_Action::GetActionText() const
{
	if (const UGSS_GameSettingAction* Action = Cast<UGSS_GameSettingAction>(Setting)) return Action->GetActionText();
	return FText::GetEmpty();
}

void UGSS_GameSettingListEntry_Action::SetSetting(UGSS_GameSetting* InSetting)
{
	ActionSetting = Cast<UGSS_GameSettingAction>(InSetting);
	Super::SetSetting(InSetting);
	if (ActionSetting)
	{
		OnSettingAssigned(ActionSetting->GetActionText());
	}
}

void UGSS_GameSettingListEntry_Action::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Button_Action)
	{
		Button_Action->OnClicked().AddUObject(this, &ThisClass::HandleActionButtonClicked);
	}
}

void UGSS_GameSettingListEntry_Action::NativeOnEntryReleased()
{
	ActionSetting = nullptr;
	Super::NativeOnEntryReleased();
}
bool UGSS_GameSettingListEntry_Action::ExecuteAction()
{
	if (UGSS_GameSettingAction* Action = Cast<UGSS_GameSettingAction>(Setting); Action && CanInteractWithSetting()) { Action->ExecuteAction(); return true; }
	return false;
}

void UGSS_GameSettingListEntry_Action::HandleEditStateChanged(UGSS_GameSetting* ChangedSetting)
{
	ActionSetting = Cast<UGSS_GameSettingAction>(Setting);
	Super::HandleEditStateChanged(ChangedSetting);
	if (ActionSetting)
	{
		OnSettingAssigned(ActionSetting->GetActionText());
	}
	if (Button_Action) Button_Action->SetIsEnabled(IsSettingEnabled());
}

void UGSS_GameSettingListEntry_Action::HandleActionButtonClicked()
{
	ExecuteAction();
}
FText UGSS_GameSettingListEntry_Navigation::GetNavigationText() const
{
	if (const UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting)) return Page->GetNavigationText();
	return FText::GetEmpty();
}

void UGSS_GameSettingListEntry_Navigation::SetSetting(UGSS_GameSetting* InSetting)
{
	CollectionSetting = Cast<UGSS_GameSettingCollectionPage>(InSetting);
	Super::SetSetting(InSetting);
	if (CollectionSetting)
	{
		OnSettingAssigned(CollectionSetting->GetNavigationText());
	}
}

void UGSS_GameSettingListEntry_Navigation::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Button_Navigate)
	{
		Button_Navigate->OnClicked().AddUObject(this, &ThisClass::HandleNavigationButtonClicked);
	}
}

void UGSS_GameSettingListEntry_Navigation::NativeOnEntryReleased()
{
	CollectionSetting = nullptr;
	Super::NativeOnEntryReleased();
}
bool UGSS_GameSettingListEntry_Navigation::Navigate()
{
	if (UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting); Page && CanInteractWithSetting()) { Page->ExecuteNavigation(); return true; }
	return false;
}

void UGSS_GameSettingListEntry_Navigation::HandleEditStateChanged(UGSS_GameSetting* ChangedSetting)
{
	CollectionSetting = Cast<UGSS_GameSettingCollectionPage>(Setting);
	Super::HandleEditStateChanged(ChangedSetting);
	if (CollectionSetting)
	{
		OnSettingAssigned(CollectionSetting->GetNavigationText());
	}
	if (Button_Navigate) Button_Navigate->SetIsEnabled(IsSettingEnabled());
}

void UGSS_GameSettingListEntry_Navigation::HandleNavigationButtonClicked()
{
	Navigate();
}
