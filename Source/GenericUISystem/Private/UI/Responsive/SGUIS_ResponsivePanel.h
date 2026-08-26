// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/SCompoundWidget.h"

class FArrangedChildren;
class SWidget;
struct FGeometry;

/**
 * Slate panel that flows children horizontally and can wrap to a vertical stack on small physical screens.
 * 横向排布子控件，并在物理屏幕较窄时改为纵向堆叠的 Slate 面板。
 */
class SGUIS_ResponsivePanel : public SCompoundWidget
{
public:
	typedef SGridPanel::FSlot FSlot;

	SLATE_BEGIN_ARGS(SGUIS_ResponsivePanel)
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}
	SLATE_END_ARGS()

	SGUIS_ResponsivePanel();

	void Construct(const FArguments& InArgs);

	/** Adds a content slot. / 添加一个内容槽。 */
	FSlot& AddSlot();

	/**
	 * Removes the slot that owns the given widget.
	 * 移除拥有指定控件的槽。
	 * @return Removed slot index, or INDEX_NONE. / 被移除槽的索引；未找到时为 INDEX_NONE。
	 */
	int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget);

	/** Removes all slots. / 移除全部槽。 */
	void ClearChildren();

	void EnableVerticalStacking(bool bCanVerticallyWrap);

protected:
	//~ Begin SWidget
	virtual bool CustomPrepass(float LayoutScaleMultiplier) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual float GetRelativeLayoutScale(int32 ChildIndex, float LayoutScaleMultiplier) const override;
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	//~ End SWidget

	bool ShouldWrap() const;
	void RefreshResponsiveness();
	void RefreshLayout();

	TSharedRef<SGridPanel> InnerGrid;
	TArray<SGridPanel::FSlot*> InnerSlots;

	FVector2D PhysicalScreenSize = FVector2D::ZeroVector;
	float Scale = 1.0f;

	uint8 bCanWrapVertically : 1;

	/** Viewport physical width (inches) below which children stack vertically. / 低于该视口物理宽度（英寸）时改为纵向堆叠。 */
	static constexpr float VerticalWrapPhysicalWidthInches = 7.0f;
	/** Layout scale applied after wrapping. / 折行后使用的布局缩放。 */
	static constexpr float WrappedLayoutScale = 1.5f;
};
