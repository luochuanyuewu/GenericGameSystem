// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "UI/Responsive/GUIS_ResponsivePanelSlot.h"

#include "Components/Widget.h"
#include "UI/Responsive/SGUIS_ResponsivePanel.h"
#include "Widgets/SNullWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_ResponsivePanelSlot)

UGUIS_ResponsivePanelSlot::UGUIS_ResponsivePanelSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGUIS_ResponsivePanelSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = nullptr;
}

void UGUIS_ResponsivePanelSlot::BuildSlot(const TSharedRef<SGUIS_ResponsivePanel>& ResponsivePanel)
{
	Slot = &ResponsivePanel->AddSlot()
	[
		Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
	];
}

void UGUIS_ResponsivePanelSlot::SynchronizeProperties()
{
}
