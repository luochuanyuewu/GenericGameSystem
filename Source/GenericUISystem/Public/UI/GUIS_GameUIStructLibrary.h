// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "UObject/Object.h"
#include "GUIS_GameUIStructLibrary.generated.h"

class UGUIS_GameUIContext;
class UGUIS_GameUILayout;

DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIActionExecutedDelegate, FName, ActionName);


USTRUCT(BlueprintType)
struct FGUIS_UIActionBindingHandle
{
	GENERATED_BODY()

	FName Id;

	FUIActionBindingHandle Handle;
};

USTRUCT(BlueprintType)
struct FGUIS_UIContextBindingHandle
{
	GENERATED_BODY()

	FGUIS_UIContextBindingHandle()
	{
	};

	FGUIS_UIContextBindingHandle(ULocalPlayer* InLocalPlayer, UClass* InContextClass);

	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY()
	UClass* ContextClass{nullptr};
};


USTRUCT()
struct FGUIS_RootViewportLayoutInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UGUIS_GameUILayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UGUIS_GameUIContext>> Contexts;

	UPROPERTY(Transient)
	TArray<FUIActionBindingHandle> BindingHandles;

	FGUIS_RootViewportLayoutInfo()
	{
	}

	FGUIS_RootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UGUIS_GameUILayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bAddedToViewport(bIsInViewport)
	{
	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};
