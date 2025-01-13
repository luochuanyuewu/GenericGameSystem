// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "GUIS_GameModalTypes.h"

#include "GUIS_GameModal.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;
class UGUIS_GameModalWidget;
/**
 * Definition of a modal.
 * 游戏消息框定义
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Const)
class GENERICUISYSTEM_API UGUIS_ModalDefinition : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The header of the message to display
	 * 消息框标题。
	 */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	FText Header;

	/**
	 * The body of the message to display
	 * 消息框正文。
	 */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	FText Body;

	/**
	 * Widget used to represent this modal.
	 * 使用哪一个ModalWidget来表示该消息框。
	 */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	TSoftClassPtr<UGUIS_GameModalWidget> ModalWidget;

	/**
	 * The confirm button's input action to use.
	 * 有哪些消息框操作及其对应按钮。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (ForceInlineRow, Categories = "GUIS.Modal.Action"))
	TMap<FGameplayTag, FGUIS_GameModalAction> ModalActions;
};

/**
 * Basic widget for modal.
 * @attention Widget must bind an "DynamicEntryBox" named EntryBox_Buttons to register buttons.
 * 游戏Modal基础UI
 * @注意 Widget必须添加一个名为EntryBox_Buttons的"DynamicEntryBox"来展示按钮。
 */
UCLASS(Abstract, meta = (Category = "Generic UI System"))
class GENERICUISYSTEM_API UGUIS_GameModalWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UGUIS_GameModalWidget();

	virtual void SetupModal(const UGUIS_ModalDefinition *ModalDefinition, FGUIS_ModalActionResultSignature ModalActionCallback);

	/**
	 * Close current modal with result. 以指定结果关闭此Modal
	 * @param ModalActionResult The modal action result. Modal操作结果。
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS", meta = (Categories = "UI.Modal.Action,GUIS.Modal.Action"))
	void CloseModal(FGameplayTag ModalActionResult);

	virtual void KillModal();

protected:
	/**
	 * Implement this event to apply data from descriptor to this modal ui elements. 通过实现此事件把来自Descriptor的信息传递到UI上。
	 * @param ModalDefinition The object contains required information about this modal. 包含了这个Modal所需信息的对象。
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="GUIS")
	void OnSetupModal(const UGUIS_ModalDefinition *ModalDefinition);

	FGUIS_ModalActionResultSignature OnModalActionCallback;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UDynamicEntryBox> EntryBox_Buttons;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Header;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Body;
};
