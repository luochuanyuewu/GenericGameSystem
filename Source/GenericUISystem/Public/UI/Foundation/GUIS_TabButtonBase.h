// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUIS_ButtonBase.h"
#include "GUIS_TabListWidgetBase.h"
#include "GUIS_TabButtonBase.generated.h"

class UCommonLazyImage;

/**
 * Button used to switch between different tabs.
 * 这是用于切换到不同选项卡的按钮。
 */
UCLASS(Abstract, Blueprintable, meta = (Category = "Generic UI", DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_TabButtonBase : public UGUIS_ButtonBase, public IGUIS_TabButtonInterface
{
	GENERATED_BODY()

	// void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	// void SetIconBrush(const FSlateBrush& Brush);

	// virtual void SetTabLabelInfo_Implementation(const FGUIS_TabDescriptor& TabLabelInfo) override;

	// UPROPERTY(meta = (BindWidgetOptional))
	// TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
