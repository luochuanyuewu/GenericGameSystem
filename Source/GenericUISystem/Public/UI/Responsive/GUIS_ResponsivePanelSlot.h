// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Components/PanelSlot.h"
#include "Widgets/Layout/SGridPanel.h"

#include "GUIS_ResponsivePanelSlot.generated.h"

class SGUIS_ResponsivePanel;

/**
 * Slot used by UGUIS_ResponsivePanel.
 * UGUIS_ResponsivePanel 使用的槽。
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_ResponsivePanelSlot : public UPanelSlot
{
	GENERATED_BODY()

public:
	UGUIS_ResponsivePanelSlot(const FObjectInitializer& ObjectInitializer);

	void BuildSlot(const TSharedRef<SGUIS_ResponsivePanel>& ResponsivePanel);

	//~ Begin UPanelSlot
	virtual void SynchronizeProperties() override;
	//~ End UPanelSlot

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	SGridPanel::FSlot* Slot = nullptr;
};
