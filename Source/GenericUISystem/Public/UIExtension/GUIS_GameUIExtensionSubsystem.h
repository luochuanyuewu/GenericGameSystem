// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/WorldSubsystem.h"
#include "GUIS_GameUIExtensionSubsystem.generated.h"

struct FGUIS_GameUIExtRequest;
template <typename T>
class TSubclassOf;

class FSubsystemCollectionBase;
class UUserWidget;
struct FFrame;

// Match rule for extension points
UENUM(BlueprintType)
enum class EGUIS_GameUIExtPointMatchType : uint8
{
	// An exact match will only receive extensions with exactly the same point
	// (e.g., registering for "A.B" will match a broadcast of A.B but not A.B.C)
	ExactMatch,

	// A partial match will receive any extensions rooted in the same point
	// (e.g., registering for "A.B" will match a broadcast of A.B as well as A.B.C)
	PartialMatch
};

// Match rule for extension points
UENUM(BlueprintType)
enum class EGUIS_GameUIExtAction : uint8
{
	Added,
	Removed
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EGUIS_GameUIExtAction Action, const FGUIS_GameUIExtRequest& Request);

/*
 *
 */
struct FGUIS_GameUIExt : TSharedFromThis<FGUIS_GameUIExt>
{
public:
	/** The extension point this extension is intended for. */
	FGameplayTag ExtensionPointTag;
	int32 Priority = INDEX_NONE;
	TWeakObjectPtr<UObject> ContextObject;
	//Kept alive by UGUIS_ExtensionSubsystem::AddReferencedObjects
	TObjectPtr<UObject> Data = nullptr;
};

/**
 * 
 */
struct FGUIS_GameUIExtPoint : TSharedFromThis<FGUIS_GameUIExtPoint>
{
public:
	FGameplayTag ExtensionPointTag;
	TWeakObjectPtr<UObject> ContextObject;
	EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType = EGUIS_GameUIExtPointMatchType::ExactMatch;
	TArray<TObjectPtr<UClass>> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	// Tests if the extension and the extension point match up, if they do then this extension point should learn
	// about this extension.
	bool DoesExtensionPassContract(const FGUIS_GameUIExt* Extension) const;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICUISYSTEM_API FGUIS_GameUIExtPointHandle
{
	GENERATED_BODY()

public:
	FGUIS_GameUIExtPointHandle()
	{
	}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==(const FGUIS_GameUIExtPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FGUIS_GameUIExtPointHandle& Other) const { return !operator==(Other); }

	friend uint32 GetTypeHash(const FGUIS_GameUIExtPointHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UGUIS_ExtensionSubsystem> ExtensionSource;

	TSharedPtr<FGUIS_GameUIExtPoint> DataPtr;

	friend UGUIS_ExtensionSubsystem;

	FGUIS_GameUIExtPointHandle(UGUIS_ExtensionSubsystem* InExtensionSource, const TSharedPtr<FGUIS_GameUIExtPoint>& InDataPtr) : ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FGUIS_GameUIExtPointHandle> : public TStructOpsTypeTraitsBase2<FGUIS_GameUIExtPointHandle>
{
	enum
	{
		WithCopy = true, // This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct GENERICUISYSTEM_API FGUIS_GameUIExtHandle
{
	GENERATED_BODY()

public:
	FGUIS_GameUIExtHandle()
	{
	}

	void Unregister();

	bool IsValid() const { return DataPtr.IsValid(); }

	bool operator==(const FGUIS_GameUIExtHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FGUIS_GameUIExtHandle& Other) const { return !operator==(Other); }

	friend FORCEINLINE uint32 GetTypeHash(FGUIS_GameUIExtHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UGUIS_ExtensionSubsystem> ExtensionSource;

	TSharedPtr<FGUIS_GameUIExt> DataPtr;

	friend UGUIS_ExtensionSubsystem;

	FGUIS_GameUIExtHandle(UGUIS_ExtensionSubsystem* InExtensionSource, const TSharedPtr<FGUIS_GameUIExt>& InDataPtr) : ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FGUIS_GameUIExtHandle> : public TStructOpsTypeTraitsBase2<FGUIS_GameUIExtHandle>
{
	enum
	{
		WithCopy = true, // This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGUIS_GameUIExtRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS")
	FGUIS_GameUIExtHandle ExtensionHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="GUIS")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="GUIS")
	int32 Priority = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="GUIS")
	TObjectPtr<UObject> Data = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="GUIS")
	TObjectPtr<UObject> ContextObject = nullptr;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FExtendExtensionPointDynamicDelegate, EGUIS_GameUIExtAction, Action, const FGUIS_GameUIExtRequest&, ExtensionRequest);

/**
 * 
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_ExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FGUIS_GameUIExtPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses,
	                                                  FExtendExtensionPointDelegate ExtensionCallback);
	FGUIS_GameUIExtPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType,
	                                                            const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FGUIS_GameUIExtHandle RegisterExtensionAsWidget(const FGameplayTag& ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FGUIS_GameUIExtHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FGUIS_GameUIExtHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	void UnregisterExtension(const FGUIS_GameUIExtHandle& ExtensionHandle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	void UnregisterExtensionPoint(const FGUIS_GameUIExtPointHandle& ExtensionPointHandle);

	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void NotifyExtensionPointOfExtensions(TSharedPtr<FGUIS_GameUIExtPoint>& ExtensionPoint);
	void NotifyExtensionPointsOfExtension(EGUIS_GameUIExtAction Action, TSharedPtr<FGUIS_GameUIExt>& Extension);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension", meta = (DisplayName = "Register Extension Point"))
	FGUIS_GameUIExtPointHandle K2_RegisterExtensionPoint(FGameplayTag ExtensionPointTag, EGUIS_GameUIExtPointMatchType ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses,
	                                                     FExtendExtensionPointDynamicDelegate ExtensionCallback);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension", meta = (DisplayName = "Register Extension (Widget)"))
	FGUIS_GameUIExtHandle K2_RegisterExtensionAsWidget(FGameplayTag ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority = -1);

	/**
	 * Registers the widget (as data) for a specific player.  This means the extension points will receive a UIExtensionForPlayer data object
	 * that they can look at to determine if it's for whatever they consider their player.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension", meta = (DisplayName = "Register Extension (Widget For Context)"))
	FGUIS_GameUIExtHandle K2_RegisterExtensionAsWidgetForContext(FGameplayTag ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority = -1);

	/**
	 * Registers the extension as data for any extension point that can make use of it.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension", meta = (DisplayName = "Register Extension (Data)"))
	FGUIS_GameUIExtHandle K2_RegisterExtensionAsData(FGameplayTag ExtensionPointTag, UObject* Data, int32 Priority = -1);

	/**
	 * Registers the extension as data for any extension point that can make use of it.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Extension", meta = (DisplayName = "Register Extension (Data For Context)"))
	FGUIS_GameUIExtHandle K2_RegisterExtensionAsDataForContext(FGameplayTag ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority = -1);

	FGUIS_GameUIExtRequest CreateExtensionRequest(const TSharedPtr<FGUIS_GameUIExt>& Extension);

private:
	typedef TArray<TSharedPtr<FGUIS_GameUIExtPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;

	typedef TArray<TSharedPtr<FGUIS_GameUIExt>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;
};


UCLASS()
class GENERICUISYSTEM_API UGUIS_ExtensionHandleFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGUIS_ExtensionHandleFunctionLibrary()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static void Unregister(UPARAM(ref) FGUIS_GameUIExtHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static bool IsValid(UPARAM(ref) FGUIS_GameUIExtHandle& Handle);
};

UCLASS()
class GENERICUISYSTEM_API UGUIS_ExtensionPointHandleFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGUIS_ExtensionPointHandleFunctionLibrary()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static void Unregister(UPARAM(ref) FGUIS_GameUIExtPointHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static bool IsValid(UPARAM(ref) FGUIS_GameUIExtPointHandle& Handle);
};
