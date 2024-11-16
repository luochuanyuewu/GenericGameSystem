// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameplayTagContainer.h"
#include "GUIS_GameUIFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h" // IWYU pragma: keep

#include "GUIS_GameUILayout.generated.h"

class APlayerController;
class UClass;
class UCommonActivatableWidgetContainerBase;
class ULocalPlayer;
class UObject;
struct FFrame;

/**
 * The state of an async load operation for the UI.
 */
enum class EGUIS_AsyncWidgetLayerState : uint8
{
	Canceled,
	Initialize,
	AfterPush
};

/**
 * The primary game UI layout of your game.  This widget class represents how to layout, push and display all layers
 * of the UI for a single player.  Each player in a split-screen game will receive their own primary game layout.
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_GameUILayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UGUIS_GameUILayout(const FObjectInitializer &ObjectInitializer);

	/** A dormant root layout is collapsed and responds only to persistent actions registered by the owning player */
	void SetIsDormant(bool Dormant);
	bool IsDormant() const { return bIsDormant; }

public:
	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass)
	{
		return PushWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, bSuspendInputUntilComplete, ActivatableWidgetClass, [](EGUIS_AsyncWidgetLayerState, ActivatableWidgetT *) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete, TSoftClassPtr<UCommonActivatableWidget> ActivatableWidgetClass,
															  TFunction<void(EGUIS_AsyncWidgetLayerState, ActivatableWidgetT *)> StateFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
		const FName SuspendInputToken = bSuspendInputUntilComplete ? UGUIS_GameUIFunctionLibrary::SuspendInputForPlayer(GetOwningPlayer(), NAME_PushingWidgetToLayer) : NAME_None;

		FStreamableManager &StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(ActivatableWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,
																																											[this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken]()
																																											{
																																												UGUIS_GameUIFunctionLibrary::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

																																												ActivatableWidgetT *Widget = PushWidgetToLayerStack<ActivatableWidgetT>(
																																													LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT &WidgetToInit)
																																													{ StateFunc(EGUIS_AsyncWidgetLayerState::Initialize, &WidgetToInit); });

																																												StateFunc(EGUIS_AsyncWidgetLayerState::AfterPush, Widget);
																																											}));

		// Setup a cancel delegate so that we can resume input if this handler is canceled.
		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
																				  [this, StateFunc, SuspendInputToken]()
																				  {
																					  UGUIS_GameUIFunctionLibrary::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
																					  StateFunc(EGUIS_AsyncWidgetLayerState::Canceled, nullptr);
																				  }));

		return StreamingHandle;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT *PushWidgetToLayerStack(FGameplayTag LayerName, UClass *ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT &) {});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT *PushWidgetToLayerStack(FGameplayTag LayerName, UClass *ActivatableWidgetClass, TFunctionRef<void(ActivatableWidgetT &)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived, "Only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase *Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}

	// Find the widget if it exists on any of the layers and remove it from the layer.
	void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget *ActivatableWidget);

	// Get the layer widget for the given layer tag.
	UCommonActivatableWidgetContainerBase *GetLayerWidget(FGameplayTag LayerName);

protected:
	/** Register a layer that widgets can be pushed onto. */
	UFUNCTION(BlueprintCallable, Category="GUIS")
	void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer,GUIS.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase *LayerWidget);

	virtual void OnIsDormantChanged();

	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase *Widget, bool bIsTransitioning);

private:
	bool bIsDormant = false;

	// Lets us keep track of all suspended input tokens so that multiple async UIs can be loading and we correctly suspend
	// for the duration of all of them.
	TArray<FName> SuspendInputTokens;

	// The registered layers for the primary layout.
	UPROPERTY(Transient, meta = (Categories = "UI.Layer,GUIS.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
