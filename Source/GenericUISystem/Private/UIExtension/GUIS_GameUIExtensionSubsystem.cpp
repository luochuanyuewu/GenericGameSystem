// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#include "UIExtension/GUIS_GameUIExtensionSubsystem.h"

#include "GUIS_LogChannels.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Stack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_GameUIExtensionSubsystem)

class FSubsystemCollectionBase;

//=========================================================

void FGUIS_GameUIExtPointHandle::Unregister()
{
	if (UGUIS_ExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtensionPoint(*this);
	}
}

//=========================================================

void FGUIS_GameUIExtHandle::Unregister()
{
	if (UGUIS_ExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterExtension(*this);
	}
}

//=========================================================

bool FGUIS_GameUIExtPoint::DoesExtensionPassContract(const FGUIS_GameUIExt* Extension) const
{
	if (UObject* DataPtr = Extension->Data)
	{
		const bool bMatchesContext =
			(ContextObject.IsExplicitlyNull() && Extension->ContextObject.IsExplicitlyNull()) ||
			ContextObject == Extension->ContextObject;

		// Make sure the contexts match.
		if (bMatchesContext)
		{
			// The data can either be the literal class of the data type, or a instance of the class type.
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}

	return false;
}

//=========================================================

void UGUIS_ExtensionSubsystem::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	Super::AddReferencedObjects(InThis, Collector);
	if (UGUIS_ExtensionSubsystem* ExtensionSubsystem = Cast<UGUIS_ExtensionSubsystem>(InThis))
	{
		for (auto MapIt = ExtensionSubsystem->ExtensionPointMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FGUIS_GameUIExtPoint>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObjects(ValueElement->AllowedDataClasses);
			}
		}

		for (auto MapIt = ExtensionSubsystem->ExtensionMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FGUIS_GameUIExt>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObject(ValueElement->Data);
			}
		}
	}
}

void UGUIS_ExtensionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGUIS_ExtensionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FGUIS_GameUIExtPointHandle UGUIS_ExtensionSubsystem::RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType,
                                                                      const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(ExtensionPointTag, nullptr, ExtensionPointTagMatchType, AllowedDataClasses, ExtensionCallback);
}

FGUIS_GameUIExtPointHandle UGUIS_ExtensionSubsystem::RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType,
                                                                                const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!ExtensionPointTag.IsValid())
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to register an invalid extension point."));
		return FGUIS_GameUIExtPointHandle();
	}

	if (!ExtensionCallback.IsBound())
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to register an invalid extension point."));
		return FGUIS_GameUIExtPointHandle();
	}

	if (AllowedDataClasses.Num() == 0)
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to register an invalid extension point."));
		return FGUIS_GameUIExtPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FGUIS_GameUIExtPoint>& Entry = List.Add_GetRef(MakeShared<FGUIS_GameUIExtPoint>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->ExtensionPointTagMatchType = ExtensionPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);

	UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension Point [%s] Registered"), *ExtensionPointTag.ToString());

	NotifyExtensionPointOfExtensions(Entry);

	return FGUIS_GameUIExtPointHandle(this, Entry);
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::RegisterExtensionAsWidget(const FGameplayTag& ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, nullptr, WidgetClass, Priority);
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (!ExtensionPointTag.IsValid())
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to register an invalid extension."));
		return FGUIS_GameUIExtHandle();
	}

	if (!Data)
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to register an invalid extension."));
		return FGUIS_GameUIExtHandle();
	}

	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FGUIS_GameUIExt>& Entry = List.Add_GetRef(MakeShared<FGUIS_GameUIExt>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	if (ContextObject)
	{
		UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension [%s] @ [%s] Registered"), *GetNameSafe(Data), *ExtensionPointTag.ToString());
	}
	else
	{
		UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension [%s] for [%s] @ [%s] Registered"), *GetNameSafe(Data), *GetNameSafe(ContextObject), *ExtensionPointTag.ToString());
	}

	NotifyExtensionPointsOfExtension(EGUIS_GameUIExtAction::Added, Entry);

	return FGUIS_GameUIExtHandle(this, Entry);
}

void UGUIS_ExtensionSubsystem::NotifyExtensionPointOfExtensions(TSharedPtr<FGUIS_GameUIExtPoint>& ExtensionPoint)
{
	for (FGameplayTag Tag = ExtensionPoint->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionList* ListPtr = ExtensionMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionList ExtensionArray(*ListPtr);

			for (const TSharedPtr<FGUIS_GameUIExt>& Extension : ExtensionArray)
			{
				if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
				{
					FGUIS_GameUIExtRequest Request = CreateExtensionRequest(Extension);
					ExtensionPoint->Callback.ExecuteIfBound(EGUIS_GameUIExtAction::Added, Request);
				}
			}
		}

		if (ExtensionPoint->ExtensionPointTagMatchType == EGUIS_GameUIExtPointMatchType::ExactMatch)
		{
			break;
		}
	}
}

void UGUIS_ExtensionSubsystem::NotifyExtensionPointsOfExtension(EGUIS_GameUIExtAction Action, TSharedPtr<FGUIS_GameUIExt>& Extension)
{
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Extension->ExtensionPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionPointList ExtensionPointArray(*ListPtr);

			for (const TSharedPtr<FGUIS_GameUIExtPoint>& ExtensionPoint : ExtensionPointArray)
			{
				if (bOnInitialTag || (ExtensionPoint->ExtensionPointTagMatchType == EGUIS_GameUIExtPointMatchType::PartialMatch))
				{
					if (ExtensionPoint->DoesExtensionPassContract(Extension.Get()))
					{
						FGUIS_GameUIExtRequest Request = CreateExtensionRequest(Extension);
						ExtensionPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}

		bOnInitialTag = false;
	}
}

void UGUIS_ExtensionSubsystem::UnregisterExtension(const FGUIS_GameUIExtHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("Trying to unregister an extension that's not from this extension subsystem."));

		TSharedPtr<FGUIS_GameUIExt> Extension = ExtensionHandle.DataPtr;
		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			if (Extension->ContextObject.IsExplicitlyNull())
			{
				UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension [%s] @ [%s] Unregistered"), *GetNameSafe(Extension->Data), *Extension->ExtensionPointTag.ToString());
			}
			else
			{
				UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension [%s] for [%s] @ [%s] Unregistered"), *GetNameSafe(Extension->Data), *GetNameSafe(Extension->ContextObject.Get()),
				       *Extension->ExtensionPointTag.ToString());
			}

			NotifyExtensionPointsOfExtension(EGUIS_GameUIExtAction::Removed, Extension);

			ListPtr->RemoveSwap(Extension);

			if (ListPtr->Num() == 0)
			{
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
	}
	else
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to unregister an invalid Handle."));
	}
}

void UGUIS_ExtensionSubsystem::UnregisterExtensionPoint(const FGUIS_GameUIExtPointHandle& ExtensionPointHandle)
{
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);

		const TSharedPtr<FGUIS_GameUIExtPoint> ExtensionPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(ExtensionPoint->ExtensionPointTag))
		{
			UE_LOG(LogGUIS_Extension, Verbose, TEXT("Extension Point [%s] Unregistered"), *ExtensionPoint->ExtensionPointTag.ToString());

			ListPtr->RemoveSwap(ExtensionPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionPointMap.Remove(ExtensionPoint->ExtensionPointTag);
			}
		}
	}
	else
	{
		UE_LOG(LogGUIS_Extension, Warning, TEXT("Trying to unregister an invalid Handle."));
	}
}

FGUIS_GameUIExtRequest UGUIS_ExtensionSubsystem::CreateExtensionRequest(const TSharedPtr<FGUIS_GameUIExt>& Extension)
{
	FGUIS_GameUIExtRequest Request;
	Request.ExtensionHandle = FGUIS_GameUIExtHandle(this, Extension);
	Request.ExtensionPointTag = Extension->ExtensionPointTag;
	Request.Priority = Extension->Priority;
	Request.Data = Extension->Data;
	Request.ContextObject = Extension->ContextObject.Get();

	return Request;
}

FGUIS_GameUIExtPointHandle UGUIS_ExtensionSubsystem::K2_RegisterExtensionPoint(FGameplayTag ExtensionPointTag, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses,
                                                                         FExtendExtensionPointDynamicDelegate ExtensionCallback)
{
	return RegisterExtensionPoint(ExtensionPointTag, ExtensionPointTagMatchType, AllowedDataClasses, FExtendExtensionPointDelegate::CreateWeakLambda(
		                              ExtensionCallback.GetUObject(), [this, ExtensionCallback](EGUIS_GameUIExtAction Action, const FGUIS_GameUIExtRequest& Request)
		                              {
			                              ExtensionCallback.ExecuteIfBound(Action, Request);
		                              }));
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::K2_RegisterExtensionAsWidget(FGameplayTag ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsWidget(ExtensionPointTag, WidgetClass, Priority);
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::K2_RegisterExtensionAsWidgetForContext(FGameplayTag ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority)
{
	if (ContextObject)
	{
		return RegisterExtensionAsWidgetForContext(ExtensionPointTag, ContextObject, WidgetClass, Priority);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("A null ContextObject was passed to Register Extension (Widget For Context)"), ELogVerbosity::Error);
		return FGUIS_GameUIExtHandle();
	}
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::K2_RegisterExtensionAsData(FGameplayTag ExtensionPointTag, UObject* Data, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, nullptr, Data, Priority);
}

FGUIS_GameUIExtHandle UGUIS_ExtensionSubsystem::K2_RegisterExtensionAsDataForContext(FGameplayTag ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (ContextObject)
	{
		return RegisterExtensionAsData(ExtensionPointTag, ContextObject, Data, Priority);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("A null ContextObject was passed to Register Extension (Data For Context)"), ELogVerbosity::Error);
		return FGUIS_GameUIExtHandle();
	}
}

//=========================================================

void UGUIS_ExtensionHandleFunctionLibrary::Unregister(FGUIS_GameUIExtHandle& Handle)
{
	Handle.Unregister();
}

bool UGUIS_ExtensionHandleFunctionLibrary::IsValid(FGUIS_GameUIExtHandle& Handle)
{
	return Handle.IsValid();
}

//=========================================================

void UGUIS_ExtensionPointHandleFunctionLibrary::Unregister(FGUIS_GameUIExtPointHandle& Handle)
{
	Handle.Unregister();
}

bool UGUIS_ExtensionPointHandleFunctionLibrary::IsValid(FGUIS_GameUIExtPointHandle& Handle)
{
	return Handle.IsValid();
}
