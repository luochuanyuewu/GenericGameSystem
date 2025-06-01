// // Copyright 2024 https://yuewu.dev/en  All Rights Reserved.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "CommonUserWidget.h"
// #include "Components/ListView.h"
// #include "GUIS_ListViewContainer.generated.h"
//
// /**
//  * List view container is a wrapper of (ListView,TileView,CommonListView,CommonTitle).
//  * Primarily purpose of this wrapper is to abstract common logic or any list-like item manipulation.
//  * 列表视图容器是(ListView,TileView,CommonListView,CommonTitle)的包装，
//  * 此包装的主要目的是抽象出任何类列表项操作的通用逻辑。
//  */
// UCLASS(Abstract, meta = (Category = "Generic UI"))
// class GENERICUISYSTEM_API UGUIS_ListViewContainer : public UCommonUserWidget
// {
// 	GENERATED_BODY()
//
// public:
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|ViewContainer")
// 	UListView* GetView() const;
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|ViewContainer", meta=(DeterminesOutputType="DesiredClass", DynamicOutputParam="ReturnValue"))
// 	UListView* GetTypedView(TSubclassOf<UListView> DesiredClass) const;
//
// protected:
// 	virtual void NativeOnInitialized() override;
// 	virtual void NativeConstruct() override;
// 	virtual void NativeDestruct() override;
//
// 	void RegisterViewCallbacks(bool bRegister);
//
// private:
// 	void HandleItemClicked(UObject* Item);
// 	void HandleItemDoubleClicked(UObject* Item);
// 	void HandleSelectionChanged(UObject* FirstSelectedItem);
// 	void HandleHoveredChanged(UObject* Item, bool bIsHovered);
// 	void HandleScrolledIntoView(UObject* Item, UUserWidget& EntryWidget);
//
// 	// UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Entry Initialized"))
// 	// FOnListEntryInitializedDynamic BP_OnEntryInitialized;
//
// 	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Clicked"))
// 	FSimpleListItemEventDynamic BP_OnItemClicked;
//
// 	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Double Clicked"))
// 	FSimpleListItemEventDynamic BP_OnItemDoubleClicked;
//
// 	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Is Hovered Changed"))
// 	FOnItemIsHoveredChangedDynamic BP_OnItemIsHoveredChanged;
//
// 	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Selection Changed"))
// 	FOnListItemSelectionChangedDynamic BP_OnItemSelectionChanged;
//
// 	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Item Scrolled Into View"))
// 	FOnListItemScrolledIntoViewDynamic BP_OnItemScrolledIntoView;
//
// 	// UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On List View Scrolled"))
// 	// FOnListViewScrolledDynamic BP_OnListViewScrolled;
//
// protected:
// 	UFUNCTION(BlueprintNativeEvent, Category="GAIS|ViewContainer")
// 	void OnItemClicked(UObject* Item);
//
// 	UFUNCTION(BlueprintNativeEvent, Category="GAIS|ViewContainer")
// 	void OnItemDoubleClicked(UObject* Item);
//
// 	UFUNCTION(BlueprintNativeEvent, Category="GAIS|ViewContainer")
// 	void OnItemSelectionChanged(UObject* Item, bool bIsSelected);
//
// 	UFUNCTION(BlueprintNativeEvent, Category="GAIS|ViewContainer")
// 	void OnItemIsHoveredChanged(UObject* Item, bool bIsHovered);
//
// 	UFUNCTION(BlueprintNativeEvent, Category="GAIS|ViewContainer")
// 	void OnItemScrolledIntoView(UObject* Item, UUserWidget* EntryWidget);
//
// 	/** Image to be used as the background of the joystick */
// 	UPROPERTY(BlueprintReadWrite, Category="GUIS|ViewContainer", meta = (BindWidgetOptional))
// 	TObjectPtr<UListView> ListView;
//
// #if WITH_EDITOR
// 	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
// #endif
// };
