// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "GUIS_GameModalTypes.h"

#include "GUIS_GameModal.generated.h"

class UDynamicEntryBox;
class UGUIS_GameModalWidget;
/**
 * 游戏对话框描述对象
 */
UCLASS(BlueprintType, Blueprintable, Const)
class GENERICUISYSTEM_API UGUIS_ModalDefinition : public UObject
{
	GENERATED_BODY()

public:
	/** The header of the message to display */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	FText Header;

	/** The body of the message to display */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	FText Body;

	/** Widget used to represent this modal. */
	UPROPERTY(EditAnywhere, Category="GUIS", BlueprintReadWrite)
	TSoftClassPtr<UGUIS_GameModalWidget> ModalWidget;

	/** The confirm button's input action to use. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS", meta = (ForceInlineRow, Categories = "GUIS.Modal.Action"))
	TMap<FGameplayTag, FGUIS_GameModalAction> ModalActions;
};

/**
 * 游戏Modal基础UI
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
};
