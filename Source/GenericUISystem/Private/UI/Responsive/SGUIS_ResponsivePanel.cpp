// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "UI/Responsive/SGUIS_ResponsivePanel.h"

#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Layout/ArrangedChildren.h"
#include "Widgets/SViewport.h"

SGUIS_ResponsivePanel::SGUIS_ResponsivePanel()
	: InnerGrid(SNew(SGridPanel))
	, bCanWrapVertically(true)
{
	SetCanTick(false);
	bCanSupportFocus = false;
	bHasCustomPrepass = true;
	bHasRelativeLayoutScale = true;
}

void SGUIS_ResponsivePanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		InnerGrid
	];
}

SGridPanel::FSlot& SGUIS_ResponsivePanel::AddSlot()
{
	SGridPanel::FSlot* NewSlot = nullptr;
	InnerGrid->AddSlot(InnerSlots.Num(), 0)
		.Expose(NewSlot);

	check(NewSlot);
	InnerSlots.Add(NewSlot);
	RefreshLayout();

	return *NewSlot;
}

int32 SGUIS_ResponsivePanel::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	int32 RemovedIndex = INDEX_NONE;
	for (int32 SlotIdx = 0; SlotIdx < InnerSlots.Num(); ++SlotIdx)
	{
		if (SlotWidget == InnerSlots[SlotIdx]->GetWidget())
		{
			InnerSlots.RemoveAt(SlotIdx);
			RemovedIndex = SlotIdx;
			break;
		}
	}

	if (InnerGrid->RemoveSlot(SlotWidget))
	{
		RefreshLayout();
		return RemovedIndex;
	}

	return INDEX_NONE;
}

void SGUIS_ResponsivePanel::ClearChildren()
{
	InnerSlots.Reset();
	InnerGrid->ClearChildren();
	RefreshLayout();
}

void SGUIS_ResponsivePanel::EnableVerticalStacking(const bool bCanVerticallyWrap)
{
	if (bCanWrapVertically == bCanVerticallyWrap)
	{
		return;
	}

	bCanWrapVertically = bCanVerticallyWrap;
	RefreshLayout();
	Invalidate(EInvalidateWidgetReason::Layout);
}

bool SGUIS_ResponsivePanel::CustomPrepass(float LayoutScaleMultiplier)
{
	RefreshResponsiveness();
	return true;
}

void SGUIS_ResponsivePanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(
		ChildSlot.GetWidget(),
		FVector2D::ZeroVector,
		AllottedGeometry.GetLocalSize() / Scale,
		Scale
	));
}

FVector2D SGUIS_ResponsivePanel::ComputeDesiredSize(float InLayoutScale) const
{
	return SCompoundWidget::ComputeDesiredSize(InLayoutScale) * Scale;
}

float SGUIS_ResponsivePanel::GetRelativeLayoutScale(int32 ChildIndex, float LayoutScaleMultiplier) const
{
	return Scale;
}

bool SGUIS_ResponsivePanel::ShouldWrap() const
{
	if (PhysicalScreenSize.IsZero() || !bCanWrapVertically)
	{
		return false;
	}

	return PhysicalScreenSize.X < VerticalWrapPhysicalWidthInches;
}

void SGUIS_ResponsivePanel::RefreshResponsiveness()
{
	PhysicalScreenSize = FVector2D::ZeroVector;

	const TSharedPtr<SViewport> GameViewport = FSlateApplication::Get().GetGameViewport();
	if (GameViewport.IsValid())
	{
		const TSharedPtr<ISlateViewport> ViewportInterface = GameViewport->GetViewportInterface().Pin();
		if (ViewportInterface.IsValid())
		{
			const FIntPoint ViewportSize = ViewportInterface->GetSize();

			int32 ScreenDensity = 0;
			FPlatformApplicationMisc::GetPhysicalScreenDensity(ScreenDensity);

			if (ScreenDensity != 0)
			{
				PhysicalScreenSize = FVector2D(ViewportSize.X, ViewportSize.Y) / static_cast<float>(ScreenDensity);
			}
		}
	}

	const float NewScale = ShouldWrap() ? WrappedLayoutScale : 1.0f;
	if (!FMath::IsNearlyEqual(NewScale, Scale))
	{
		Scale = NewScale;
		RefreshLayout();
		Invalidate(EInvalidateWidgetReason::Prepass);
	}
}

void SGUIS_ResponsivePanel::RefreshLayout()
{
	const bool bShouldWrap = ShouldWrap();

	InnerGrid->ClearFill();

	for (int32 SlotIdx = 0; SlotIdx < InnerSlots.Num(); ++SlotIdx)
	{
		InnerSlots[SlotIdx]->SetColumn(bShouldWrap ? 0 : SlotIdx);
		InnerSlots[SlotIdx]->SetRow(bShouldWrap ? SlotIdx : 0);

		if (!bShouldWrap)
		{
			InnerGrid->SetColumnFill(SlotIdx, 1.0f);
		}
	}

	if (bShouldWrap)
	{
		InnerGrid->SetColumnFill(0, 1.0f);
	}
}
