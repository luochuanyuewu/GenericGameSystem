// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GGS_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class GENERICGAMESYSTEM_API UGGS_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Optional interface for actors to respond to interaction events.
 * Typically used for visual feedback or interaction-specific logic.
 * 角色响应交互事件的可选接口。通常用于视觉反馈或特定的交互逻辑。
 */
class GENERICGAMESYSTEM_API IGGS_InteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Return an optional friendly name of this interactable actor.
	 * 返回一个该Actor的可选友好名称。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	FText GetInteractionDisplayName() const;

	/**
	 * Called when this actor is selected by the interaction system
	 * 当该Actor被交互系统选择时调用。
	 * @param Instigator Usually the player 通常是玩家。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	void OnInteractionSelected(AActor* Instigator);

	/**
	 * Called when this actor is deselected by the interaction system
	 * 当交互系统取消选择该选项时调用。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	void OnInteractionDeselected(AActor* Instigator);

	/**
	 * Called when interaction with this actor starts
	 * 与该Actor的交互结束时调用。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	void OnInteractionStarted(AActor* Instigator);

	/**
	 * Called when interaction with this actor ends
	 * 与该Actor的交互结束时调用。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	void OnInteractionEnded(AActor* Instigator);

	/**
	 * Called when one of the interaction option selected by instigator.
	 * @details For example, when a shop option was selected, the merchant can say something like "Buy GGF, I have everything you want. ~V~"
	 * 当其中一个选项被发起者选中时调用。
	 * @细节 比如，当“购物”选项被选中时，商人可能说一句：“购买通用游戏框架吧，你想要的我都有。”
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GGS|Interaction")
	void OnInteractionOptionSelected(AActor* Instigator, int32 OptionIndex);
};
