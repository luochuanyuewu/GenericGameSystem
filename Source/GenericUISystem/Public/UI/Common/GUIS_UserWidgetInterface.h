// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GUIS_UserWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGUIS_UserWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface adds frequently used api for regular UserWidget.
 * @attention Designed to be implemented on any UserWidget(excepting UCommonActivatableWidget, and it's derived classes.)
 * @details UserWidget with this interface will be called automatically when used as extension ui.
 */
class GENERICUISYSTEM_API IGUIS_UserWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Return the owning actor of this UserWidget. 
	 * @return The logic owner of this UserWidget.
	 * @details Although each UserWidget has an owning player, sometimes you want to have an Actor that exists in the game world as a logical owning actor of the UI and keep track of its data.
	 * 获取此UserWidget的OwningActor。
	 * @返回 此UserWidget的逻辑拥有者。
	 * @细节 虽然每个 UserWidget 都有一个自己的玩家拥有者，但有时你也希望有一个 Actor 存在于游戏世界中，作为用户界面的逻辑拥有者，并跟踪其数据。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GUIS")
	AActor* GetOwningActor();

	/**
	 * Set the owning actor of this UserWidget. 设置此UserWidget的所属Actor。
	 * @param NewOwningActor The logic owner of this UserWidget. 此UserWidget的逻辑拥有者。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GUIS")
	void SetOwningActor(AActor* NewOwningActor);

	/**
	 * Notify this UserWidget was activated.通知此UserWidget被激活。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GUIS")
	void OnActivated();

	/**
	 * Notify this UserWidget was deactivated.通知此UserWidget被禁用。
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GUIS")
	void OnDeactivated();
};
