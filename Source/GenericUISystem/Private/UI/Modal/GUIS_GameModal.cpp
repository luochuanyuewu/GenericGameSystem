// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "UI/Modal/GUIS_GameModal.h"

#include "CommonBorder.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "UI/Foundation/GUIS_ButtonBase.h"
#include "UI/Modal/GUIS_GameModalTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_GameModal)

#define LOCTEXT_NAMESPACE "GUIS_GameModal"

UGUIS_GameModalWidget::UGUIS_GameModalWidget()
{
	bIsModal = true;
}

void UGUIS_GameModalWidget::SetupModal(const UGUIS_ModalDefinition* ModalDefinition, FGUIS_ModalActionResultSignature ModalActionCallback)
{
	OnModalActionCallback = ModalActionCallback;

	EntryBox_Buttons->Reset<UGUIS_ButtonBase>([](UGUIS_ButtonBase& Button)
	{
		Button.OnClicked().Clear();
	});

	Text_Header->SetText(ModalDefinition->Header);
	Text_Body->SetText(ModalDefinition->Body);

	for (const auto& Pair : ModalDefinition->ModalActions)
	{
		UGUIS_ButtonBase* Button = EntryBox_Buttons->CreateEntry<UGUIS_ButtonBase>(!Pair.Value.ButtonType.IsNull() ? Pair.Value.ButtonType.LoadSynchronous() : nullptr);
		Button->SetTriggeringInputAction(Pair.Value.InputAction);
		Button->OnClicked().AddUObject(this, &ThisClass::CloseModal, Pair.Key);
		if (!Pair.Value.DisplayText.IsEmpty())
		{
			Button->SetButtonText(Pair.Value.DisplayText);
		}
	}

	if (ModalDefinition->CancelActionTag.IsValid())
	{
		CancelActionTag = ModalDefinition->CancelActionTag;
	}

	OnSetupModal(ModalDefinition);
}

void UGUIS_GameModalWidget::CloseModal(FGameplayTag ModalActionResult)
{
	DeactivateWidget();
	OnModalActionCallback.ExecuteIfBound(ModalActionResult);
}

void UGUIS_GameModalWidget::KillModal()
{
}

void UGUIS_GameModalWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (bUseTapToCloseZone)
	{
		Border_TapToCloseZone->OnMouseButtonDownEvent.BindDynamic(this, &ThisClass::HandleTapToCloseZoneMouseButtonDown);
	}
}

FEventReply UGUIS_GameModalWidget::HandleTapToCloseZoneMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	FEventReply Reply;
	Reply.NativeReply = FReply::Unhandled();

	if (MouseEvent.IsTouchEvent() || MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CloseModal(CancelActionTag);
		Reply.NativeReply = FReply::Handled();
	}

	return Reply;
}

#undef LOCTEXT_NAMESPACE
