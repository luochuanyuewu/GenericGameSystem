// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GUIS_ButtonBase.generated.h"

/**
 * 基础按钮
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class GENERICUISYSTEM_API UGUIS_ButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GUIS")
	void SetButtonText(const FText &InText);

protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface

	void RefreshButtonText();

	// 在PreConstruct,InputActionWidget更新，以及ButtonText变化时会触发。
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateButtonText(const FText &InText);

	// 在PreConstruct,InputActionWidget更新，以及ButtonText变化时会触发。
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateButtonStyle();

protected:
	// 不勾选的情况下，会使用来自InputActionWidget的显示文字。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (ExposeOnSpawn = true, InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (ExposeOnSpawn = true, editcondition = "bOverride_ButtonText"))
	FText ButtonText;
};
