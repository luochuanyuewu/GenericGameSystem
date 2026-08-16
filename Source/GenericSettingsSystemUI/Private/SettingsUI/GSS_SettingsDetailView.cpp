// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "SettingsUI/GSS_SettingsDetailView.h"

#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_SettingEditCondition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsDetailView)

#define LOCTEXT_NAMESPACE "GSS_SettingsDetailView"

UGSS_SettingsDetailView::UGSS_SettingsDetailView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGSS_SettingsDetailView::SetListItemObject(UObject* InListItemObject)
{
	FillSettingDetails(Cast<UGSS_GameSetting>(InListItemObject));
}

void UGSS_SettingsDetailView::FillSettingDetails(UGSS_GameSetting* InSetting)
{
	if (InSetting && InSetting == CurrentSetting)
	{
		return;
	}

	ReleaseCurrentSetting();
	CurrentSetting = InSetting;
	if (CurrentSetting)
	{
		CurrentSetting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleCurrentSettingChanged);
		CurrentSetting->OnSettingEditConditionChangedEvent.AddUObject(this, &ThisClass::HandleCurrentSettingEditableStateChanged);
	}

	Super::SetListItemObject(CurrentSetting);
	RefreshSettingDetails();
}

void UGSS_SettingsDetailView::NativeDestruct()
{
	ReleaseCurrentSetting();
	Super::NativeDestruct();
}

void UGSS_SettingsDetailView::ReleaseCurrentSetting()
{
	if (CurrentSetting)
	{
		CurrentSetting->OnSettingChangedEvent.RemoveAll(this);
		CurrentSetting->OnSettingEditConditionChangedEvent.RemoveAll(this);
		CurrentSetting = nullptr;
	}
}

void UGSS_SettingsDetailView::HandleCurrentSettingChanged(UGSS_GameSetting*, EGSS_GameSettingChangeReason)
{
	RefreshSettingDetails();
}

void UGSS_SettingsDetailView::HandleCurrentSettingEditableStateChanged(UGSS_GameSetting*)
{
	RefreshSettingDetails();
}

void UGSS_SettingsDetailView::RefreshSettingDetails()
{
	if (Text_SettingName)
	{
		Text_SettingName->SetText(CurrentSetting ? CurrentSetting->GetDisplayName() : FText::GetEmpty());
	}

	if (RichText_Description)
	{
		RichText_Description->SetText(CurrentSetting ? CurrentSetting->GetDescriptionRichText() : FText::GetEmpty());
	}

	if (RichText_DynamicDetails)
	{
		const FText DynamicDetails = CurrentSetting ? CurrentSetting->GetDynamicDetails() : FText::GetEmpty();
		RichText_DynamicDetails->SetText(DynamicDetails);
		RichText_DynamicDetails->SetVisibility(DynamicDetails.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	if (RichText_WarningDetails)
	{
		const FText WarningDetails = CurrentSetting ? CurrentSetting->GetWarningRichText() : FText::GetEmpty();
		RichText_WarningDetails->SetText(WarningDetails.IsEmpty() ? FText::GetEmpty() : FText::Format(LOCTEXT("WarningLine", "<Icon.Warning></> {0}"), WarningDetails));
		RichText_WarningDetails->SetVisibility(WarningDetails.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	if (RichText_DisabledDetails)
	{
		TArray<FText> DisabledDetailLines;
		if (const UGSS_SettingEditableState* EditableState = CurrentSetting ? CurrentSetting->GetEditState() : nullptr)
		{
			if (!EditableState->IsEnabled())
			{
				for (const FText& Reason : EditableState->GetDisabledReasons())
				{
					DisabledDetailLines.Add(FText::Format(LOCTEXT("DisabledReasonLine", "<Icon.Warning></> {0}"), Reason));
				}
			}
			if (!EditableState->GetDisabledOptions().IsEmpty())
			{
				DisabledDetailLines.Add(LOCTEXT("DisabledOptionsLine", "<Icon.Warning></> Some options are currently unavailable."));
			}
		}

		RichText_DisabledDetails->SetText(FText::Join(FText::FromString(TEXT("\n")), DisabledDetailLines));
		RichText_DisabledDetails->SetVisibility(DisabledDetailLines.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	OnSettingDetailsUpdated(CurrentSetting);
}

#undef LOCTEXT_NAMESPACE
