// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"

#include "GUIS_ActivatableWidget.generated.h"

struct FUIInputConfig;

UENUM()
enum class EGUIS_ActivatableWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

// An activatable widget that automatically drives the desired input config when activated
UCLASS(Abstract, Blueprintable)
class GENERICUISYSTEM_API UGUIS_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UGUIS_ActivatableWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GUIS|ActivatableWidget")
	void SetIsBackHandler(bool bNewState);

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

#if WITH_EDITOR
	virtual void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	EGUIS_ActivatableWidgetInputMode InputConfig = EGUIS_ActivatableWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
