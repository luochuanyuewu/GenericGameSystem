// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Components/PanelWidget.h"

#include "GUIS_ResponsivePanel.generated.h"

class SGUIS_ResponsivePanel;
class UGUIS_ResponsivePanelSlot;

/**
 * Lays children out in a horizontal row, and can stack them vertically on small physical screens.
 * 将子控件横向排布；在物理屏幕较窄时可改为纵向堆叠。
 *
 * On game viewports narrower than 7 physical inches, children wrap to a single column and the layout scale becomes 1.5.
 * 当游戏视口物理宽度小于 7 英寸时，子控件会折成单列，布局缩放变为 1.5。
 */
UCLASS(meta = (DisplayName = "Responsive Panel", Category = "Generic UI"))
class GENERICUISYSTEM_API UGUIS_ResponsivePanel : public UPanelWidget
{
	GENERATED_BODY()

public:
	UGUIS_ResponsivePanel(const FObjectInitializer& ObjectInitializer);

	/** Adds a child to this responsive panel. / 向此响应式面板添加子控件。 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	UGUIS_ResponsivePanelSlot* AddChildToResponsivePanel(UWidget* Content);

	/** Returns whether children may stack vertically on small screens. / 返回窄屏时是否允许纵向堆叠。 */
	UFUNCTION(BlueprintPure, Category = "Widget")
	bool GetCanStackVertically() const { return bCanStackVertically; }

	/** Sets whether children may stack vertically on small screens. / 设置窄屏时是否允许纵向堆叠。 */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetCanStackVertically(bool bInCanStackVertically);

#if WITH_EDITOR
	//~ Begin UWidget
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget
#endif

	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/**
	 * When true, children wrap into a vertical stack on viewports narrower than 7 physical inches.
	 * 为 true 时，视口物理宽度小于 7 英寸会将子控件改为纵向堆叠。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
	bool bCanStackVertically = true;

protected:
	//~ Begin UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	//~ End UPanelWidget

	TSharedPtr<SGUIS_ResponsivePanel> MyResponsivePanel;

	//~ Begin UWidget
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget
};
