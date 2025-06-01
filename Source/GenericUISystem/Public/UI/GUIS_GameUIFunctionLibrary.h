// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"

#include "GUIS_GameUIFunctionLibrary.generated.h"

class UGUIS_GameUILayout;
enum class ECommonInputType : uint8;
template <typename T>
class TSubclassOf;

class APlayerController;
class UCommonActivatableWidget;
class ULocalPlayer;
class UObject;
class UUserWidget;
struct FFrame;
struct FGameplayTag;

/**
 * Commons functions for Game UI.
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_GameUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGUIS_GameUIFunctionLibrary()
	{
	}

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="GUIS", meta = (WorldContext = "WidgetContextObject"))
	static ECommonInputType GetOwningPlayerInputType(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="GUIS", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="GUIS", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS", meta=(DynamicOutputParam="ReturnValue", DeterminesOutputType="WidgetClass"))
	static UCommonActivatableWidget* PushContentToUILayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer,GUIS.Layer"))
	                                                                FGameplayTag LayerName,
	                                                                UPARAM(meta = (AllowAbstract = false))
	                                                                TSubclassOf<UCommonActivatableWidget> WidgetClass);

	// UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS")
	// static void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName,
	//                                                  UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS", meta=(DefaultToSelf="ActivatableWidget"))
	static void PopContentFromUILayer(UCommonActivatableWidget* ActivatableWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS")
	static ULocalPlayer* GetLocalPlayerFromController(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS")
	static UGUIS_GameUILayout* GetGameUILayoutForPlayer(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="GUIS")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken);

	/**
	 * Returns the typed item in the owning list view that this entry is currently assigned to represent.
	 * 以指定类型的方式获取Entry要展示的Item。
	 * @param DesiredClass The type of ListItemObject.
	 * @param UserObjectListEntry Note: Visually not transmitted, but this defaults to "self". No need to hook up if calling internally.
	 */
	UFUNCTION(BlueprintPure, Category="GUIS",
		meta = (DeterminesOutputType = "DesiredClass", DynamicOutputParam = "ReturnValue", DefaultToSelf = UserObjectListEntry, DisplayName = "Get List item Object"))
	static UObject* GetTypedListItem(TScriptInterface<IUserObjectListEntry> UserObjectListEntry, TSubclassOf<UObject> DesiredClass);

	/**
	 * Returns the typed item in the owning list view that this entry is currently assigned to represent.
	 * 以指定类型的方式获取Entry要展示的Item。
	 * @param DesiredClass The type of ListItemObject.
	 * @param UserObjectListEntry Note: Visually not transmitted, but this defaults to "self". No need to hook up if calling internally.
	 * @param OutItem The typed item.
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS",
		meta = (ExpandBoolAsExecs = "ReturnValue", DeterminesOutputType = "DesiredClass", DynamicOutputParam = "OutItemObject", DefaultToSelf = UserObjectListEntry, DisplayName =
			"Get List item Object"))
	static bool GetTypedListItemSafely(TScriptInterface<IUserObjectListEntry> UserObjectListEntry, TSubclassOf<UObject> DesiredClass, UObject*& OutItem);

private:
	static int32 InputSuspensions;
};
