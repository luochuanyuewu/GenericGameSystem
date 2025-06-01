// // Copyright 2024 https://yuewu.dev/en  All Rights Reserved.
//
//
// #include "UI/Common/GUIS_ListViewContainer.h"
// #include "Components/ListView.h"
//
//
// UListView* UGUIS_ListViewContainer::GetView() const
// {
// 	return ListView;
// }
//
// UListView* UGUIS_ListViewContainer::GetTypedView(TSubclassOf<UListView> DesiredClass) const
// {
// 	if (UClass* RealClass = DesiredClass.Get())
// 	{
// 		if (ListView && ListView->IsA(RealClass))
// 		{
// 			return ListView;
// 		}
// 	}
// 	return nullptr;
// }
//
// void UGUIS_ListViewContainer::NativeOnInitialized()
// {
// 	Super::NativeOnInitialized();
// }
//
// void UGUIS_ListViewContainer::NativeConstruct()
// {
// 	Super::NativeConstruct();
// 	RegisterViewCallbacks(true);
// }
//
// void UGUIS_ListViewContainer::NativeDestruct()
// {
// 	RegisterViewCallbacks(false);
// 	Super::NativeDestruct();
// }
//
// void UGUIS_ListViewContainer::RegisterViewCallbacks(bool bRegister)
// {
// 	if (!ListView)
// 	{
// 		return;
// 	}
// 	if (bRegister)
// 	{
// 		ListView->OnItemClicked().AddUObject(this, &ThisClass::HandleItemClicked);
// 		ListView->OnItemDoubleClicked().AddUObject(this, &ThisClass::HandleItemDoubleClicked);
// 		ListView->OnItemScrolledIntoView().AddUObject(this, &ThisClass::HandleScrolledIntoView);
// 		ListView->OnItemSelectionChanged().AddUObject(this, &ThisClass::HandleSelectionChanged);
// 		ListView->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::HandleHoveredChanged);
// 	}
// 	else
// 	{
// 		ListView->OnItemClicked().RemoveAll(this);
// 		ListView->OnItemDoubleClicked().RemoveAll(this);
// 		ListView->OnItemScrolledIntoView().RemoveAll(this);
// 		ListView->OnItemSelectionChanged().RemoveAll(this);
// 		ListView->OnItemIsHoveredChanged().RemoveAll(this);
// 	}
// }
//
// void UGUIS_ListViewContainer::HandleItemClicked(UObject* Item)
// {
// 	OnItemClicked(Item);
// 	BP_OnItemClicked.Broadcast(Item);
// }
//
// void UGUIS_ListViewContainer::HandleItemDoubleClicked(UObject* Item)
// {
// 	OnItemDoubleClicked(Item);
// 	BP_OnItemDoubleClicked.Broadcast(Item);
// }
//
// void UGUIS_ListViewContainer::HandleSelectionChanged(UObject* FirstSelectedItem)
// {
// 	OnItemSelectionChanged(FirstSelectedItem, FirstSelectedItem != nullptr);
// 	BP_OnItemSelectionChanged.Broadcast(FirstSelectedItem, FirstSelectedItem != nullptr);
// }
//
// void UGUIS_ListViewContainer::HandleHoveredChanged(UObject* Item, bool bIsHovered)
// {
// 	OnItemIsHoveredChanged(Item, bIsHovered);
// 	BP_OnItemIsHoveredChanged.Broadcast(Item, bIsHovered);
// }
//
// void UGUIS_ListViewContainer::HandleScrolledIntoView(UObject* Item, UUserWidget& EntryWidget)
// {
// 	OnItemScrolledIntoView(Item, &EntryWidget);
// 	BP_OnItemScrolledIntoView.Broadcast(Item, &EntryWidget);
// }
//
// void UGUIS_ListViewContainer::OnItemClicked_Implementation(UObject* Item)
// {
// }
//
// void UGUIS_ListViewContainer::OnItemDoubleClicked_Implementation(UObject* Item)
// {
// }
//
//
// void UGUIS_ListViewContainer::OnItemSelectionChanged_Implementation(UObject* Item, bool bIsSelected)
// {
// }
//
// void UGUIS_ListViewContainer::OnItemIsHoveredChanged_Implementation(UObject* Item, bool bIsHovered)
// {
// }
//
// void UGUIS_ListViewContainer::OnItemScrolledIntoView_Implementation(UObject* Item, UUserWidget* EntryWidget)
// {
// }
//
// #if WITH_EDITOR
// #include "Misc/DataValidation.h"
//
// EDataValidationResult UGUIS_ListViewContainer::IsDataValid(class FDataValidationContext& Context) const
// {
// 	if (!GetClass()->HasAnyClassFlags(CLASS_Abstract))
// 	{
// 		if (!ListView)
// 		{
// 			Context.AddError(FText::FromString(TEXT("Missing required widget binding for ListView.")));
// 			return EDataValidationResult::Invalid;
// 		}
// 	}
// 	return Super::IsDataValid(Context);
// }
//
// #endif
