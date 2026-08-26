// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "UI/Responsive/GUIS_ResponsivePanel.h"

#include "UI/Responsive/GUIS_ResponsivePanelSlot.h"
#include "UI/Responsive/SGUIS_ResponsivePanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_ResponsivePanel)

#define LOCTEXT_NAMESPACE "GUIS_ResponsivePanel"

UGUIS_ResponsivePanel::UGUIS_ResponsivePanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UGUIS_ResponsivePanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyResponsivePanel.Reset();
}

void UGUIS_ResponsivePanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyResponsivePanel.IsValid())
	{
		MyResponsivePanel->EnableVerticalStacking(bCanStackVertically);
	}
}

void UGUIS_ResponsivePanel::SetCanStackVertically(bool bInCanStackVertically)
{
	bCanStackVertically = bInCanStackVertically;

	if (MyResponsivePanel.IsValid())
	{
		MyResponsivePanel->EnableVerticalStacking(bCanStackVertically);
	}
}

UClass* UGUIS_ResponsivePanel::GetSlotClass() const
{
	return UGUIS_ResponsivePanelSlot::StaticClass();
}

void UGUIS_ResponsivePanel::OnSlotAdded(UPanelSlot* InSlot)
{
	if (MyResponsivePanel.IsValid())
	{
		CastChecked<UGUIS_ResponsivePanelSlot>(InSlot)->BuildSlot(MyResponsivePanel.ToSharedRef());
	}
}

void UGUIS_ResponsivePanel::OnSlotRemoved(UPanelSlot* InSlot)
{
	if (MyResponsivePanel.IsValid() && InSlot->Content)
	{
		const TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyResponsivePanel->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UGUIS_ResponsivePanelSlot* UGUIS_ResponsivePanel::AddChildToResponsivePanel(UWidget* Content)
{
	return Cast<UGUIS_ResponsivePanelSlot>(Super::AddChild(Content));
}

TSharedRef<SWidget> UGUIS_ResponsivePanel::RebuildWidget()
{
	MyResponsivePanel = SNew(SGUIS_ResponsivePanel);
	MyResponsivePanel->EnableVerticalStacking(bCanStackVertically);

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UGUIS_ResponsivePanelSlot* TypedSlot = Cast<UGUIS_ResponsivePanelSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyResponsivePanel.ToSharedRef());
		}
	}

	return MyResponsivePanel.ToSharedRef();
}

#if WITH_EDITOR
const FText UGUIS_ResponsivePanel::GetPaletteCategory()
{
	return LOCTEXT("PaletteCategory", "Generic UI");
}
#endif

#undef LOCTEXT_NAMESPACE
