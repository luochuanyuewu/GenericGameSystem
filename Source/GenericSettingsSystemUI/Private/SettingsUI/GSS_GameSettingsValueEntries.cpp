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
#include "UI/Foundation/GUIS_ButtonBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsValueEntries)

TArray<FText> UGSS_GameSettingListEntry_Discrete::GetOptions() const
{
	if (DiscreteSetting)
	{
		return DiscreteSetting->GetDiscreteOptions();
	}
	return {};
}

void UGSS_GameSettingListEntry_Discrete::SetSetting(UGSS_GameSetting* InSetting)
{
	DiscreteSetting = Cast<UGSS_GameSettingValueDiscrete>(InSetting);
	Super::SetSetting(InSetting);
	RefreshDiscreteControl();
	if (DiscreteSetting)
	{
		OnSettingAssigned(DiscreteSetting);
	}
}

void UGSS_GameSettingListEntry_Discrete::OnSettingAssigned_Implementation(UGSS_GameSettingValueDiscrete*)
{
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
	return DiscreteSetting ? DiscreteSetting->GetDiscreteOptionIndex() : INDEX_NONE;
}

int32 UGSS_GameSettingListEntry_Discrete::GetDefaultOptionIndex() const
{
	return DiscreteSetting ? DiscreteSetting->GetDiscreteOptionDefaultIndex() : INDEX_NONE;
}

bool UGSS_GameSettingListEntry_Discrete::SelectOption(int32 OptionIndex)
{
	if (DiscreteSetting && CanInteractWithSetting() && DiscreteSetting->GetDiscreteOptions().IsValidIndex(OptionIndex))
	{
		DiscreteSetting->SetDiscreteOptionByIndex(OptionIndex);
		return true;
	}
	return false;
}

void UGSS_GameSettingListEntry_Discrete::OnSettingChanged()
{
	RefreshDiscreteControl();
}

void UGSS_GameSettingListEntry_Discrete::HandleEditConditionChanged(UGSS_GameSetting* InSetting)
{
	Super::HandleEditConditionChanged(InSetting);
	RefreshDiscreteControl();
}

void UGSS_GameSettingListEntry_Discrete::RefreshEditableState_Implementation(const FGSS_GameSettingEditableState& InEditableState)
{
	Super::RefreshEditableState_Implementation(InEditableState);
	const bool bEnabled = InEditableState.IsEnabled();
	if (Panel_Value)
	{
		Panel_Value->SetIsEnabled(bEnabled);
	}
	if (Rotator_SettingValue)
	{
		Rotator_SettingValue->SetIsEnabled(bEnabled);
	}
	if (Button_Decrease)
	{
		Button_Decrease->SetIsEnabled(bEnabled);
	}
	if (Button_Increase)
	{
		Button_Increase->SetIsEnabled(bEnabled);
	}
}

void UGSS_GameSettingListEntry_Discrete::RefreshDiscreteControl_Implementation()
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
	return ScalarSetting ? ScalarSetting->GetValueNormalized() : 0.0;
}

void UGSS_GameSettingListEntry_Scalar::SetSetting(UGSS_GameSetting* InSetting)
{
	ScalarSetting = Cast<UGSS_GameSettingValueScalar>(InSetting);
	Super::SetSetting(InSetting);
	RefreshScalarControl();
	if (ScalarSetting)
	{
		OnSettingAssigned(ScalarSetting);
	}
}

void UGSS_GameSettingListEntry_Scalar::OnSettingAssigned_Implementation(UGSS_GameSettingValueScalar*)
{
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
	return ScalarSetting ? ScalarSetting->GetFormattedText() : FText::GetEmpty();
}

bool UGSS_GameSettingListEntry_Scalar::GetDefaultNormalizedValue(double& OutValue) const
{
	if (ScalarSetting)
	{
		if (const TOptional<double> DefaultValue = ScalarSetting->GetDefaultValueNormalized(); DefaultValue.IsSet())
		{
			OutValue = DefaultValue.GetValue();
			return true;
		}
	}
	OutValue = 0.0;
	return false;
}

double UGSS_GameSettingListEntry_Scalar::GetNormalizedStepSize() const
{
	return ScalarSetting ? ScalarSetting->GetNormalizedStepSize() : 0.0;
}

bool UGSS_GameSettingListEntry_Scalar::SetNormalizedValue(double Value)
{
	if (ScalarSetting && CanInteractWithSetting())
	{
		ScalarSetting->SetValueNormalized(Value);
		return true;
	}
	return false;
}

void UGSS_GameSettingListEntry_Scalar::OnSettingChanged()
{
	RefreshScalarControl();
}

void UGSS_GameSettingListEntry_Scalar::RefreshEditableState_Implementation(const FGSS_GameSettingEditableState& InEditableState)
{
	Super::RefreshEditableState_Implementation(InEditableState);
	const bool bEnabled = InEditableState.IsEnabled();
	if (Panel_Value)
	{
		Panel_Value->SetIsEnabled(bEnabled);
	}
	if (Slider_SettingValue)
	{
		Slider_SettingValue->SetIsEnabled(bEnabled);
	}
}

void UGSS_GameSettingListEntry_Scalar::RefreshScalarControl_Implementation()
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
	
	OnDefaultValueChanged(DefaultValue.IsSet()?DefaultValue.GetValue():-1.0f);
	
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
	return ActionSetting ? ActionSetting->GetActionText() : FText::GetEmpty();
}

void UGSS_GameSettingListEntry_Action::SetSetting(UGSS_GameSetting* InSetting)
{
	ActionSetting = Cast<UGSS_GameSettingAction>(InSetting);
	Super::SetSetting(InSetting);
	if (ActionSetting)
	{
		OnSettingAssigned(ActionSetting);
	}
}

void UGSS_GameSettingListEntry_Action::OnSettingAssigned_Implementation(UGSS_GameSettingAction* AssignedSetting)
{
	if (Button_Action && AssignedSetting)
	{
		Button_Action->SetButtonText(AssignedSetting->GetActionText());
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
	if (ActionSetting && CanInteractWithSetting())
	{
		ActionSetting->ExecuteAction();
		return true;
	}
	return false;
}

void UGSS_GameSettingListEntry_Action::RefreshEditableState_Implementation(const FGSS_GameSettingEditableState& InEditableState)
{
	Super::RefreshEditableState_Implementation(InEditableState);
	if (Button_Action)
	{
		Button_Action->SetIsEnabled(InEditableState.IsEnabled());
	}
}

void UGSS_GameSettingListEntry_Action::HandleActionButtonClicked()
{
	ExecuteAction();
}

FText UGSS_GameSettingListEntry_Navigation::GetNavigationText() const
{
	return CollectionSetting ? CollectionSetting->GetNavigationText() : FText::GetEmpty();
}

void UGSS_GameSettingListEntry_Navigation::SetSetting(UGSS_GameSetting* InSetting)
{
	CollectionSetting = Cast<UGSS_GameSettingCollectionPage>(InSetting);
	Super::SetSetting(InSetting);
	if (CollectionSetting)
	{
		OnSettingAssigned(CollectionSetting);
	}
}

void UGSS_GameSettingListEntry_Navigation::OnSettingAssigned_Implementation(UGSS_GameSettingCollectionPage* AssignedSetting)
{
	if (Button_Navigate && AssignedSetting)
	{
		Button_Navigate->SetButtonText(AssignedSetting->GetNavigationText());
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
	if (CollectionSetting && CanInteractWithSetting())
	{
		CollectionSetting->ExecuteNavigation();
		return true;
	}
	return false;
}

void UGSS_GameSettingListEntry_Navigation::RefreshEditableState_Implementation(const FGSS_GameSettingEditableState& InEditableState)
{
	Super::RefreshEditableState_Implementation(InEditableState);
	if (Button_Navigate)
	{
		Button_Navigate->SetIsEnabled(InEditableState.IsEnabled());
	}
}

void UGSS_GameSettingListEntry_Navigation::HandleNavigationButtonClicked()
{
	Navigate();
}
