// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#include "UI/GUIS_GameUISubsystem.h"
#include "GUIS_GenericUISystemSettings.h"
#include "CommonUserWidget.h"
#include "GUIS_LogChannels.h"
#include "Engine/GameInstance.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/GUIS_GameUIContext.h"
#include "UI/GUIS_GameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_GameUISubsystem)

class FSubsystemCollectionBase;
class UClass;

FGUIS_UIContextBindingHandle::FGUIS_UIContextBindingHandle(ULocalPlayer* InLocalPlayer, UClass* InContextClass)
{
	LocalPlayer = InLocalPlayer;
	ContextClass = InContextClass;
}

void UGUIS_GameUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CurrentPolicy && !UGUIS_GenericUISystemSettings::Get()->GameUIPolicyClass.IsNull())
	{
		TSubclassOf<UGUIS_GameUIPolicy> PolicyClass = UGUIS_GenericUISystemSettings::Get()->GameUIPolicyClass.LoadSynchronous();
		if (PolicyClass)
		{
			SwitchToPolicy(NewObject<UGUIS_GameUIPolicy>(this, PolicyClass));
		}
		else
		{
			UE_LOG(LogGUIS, Error, TEXT("GUIS_GameUISubsystem::Initialize Failed, Missing GameUIPolicyClass in GenericUISystemSettings!!!"));
		}
	}
}

void UGUIS_GameUISubsystem::Deinitialize()
{
	Super::Deinitialize();

	SwitchToPolicy(nullptr);
}

bool UGUIS_GameUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		return false;
	}

	TArray<UClass*> ChildClasses;
	GetDerivedClasses(GetClass(), ChildClasses, false);

	if (ChildClasses.Num() == 0)
	{
		UE_LOG(LogGUIS, Display, TEXT("No override implementation found for UGUIS_GameUISubsystem, So creating it."))
		return true;
	}
	return false;
}

void UGUIS_GameUISubsystem::AddPlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerAdded(LocalPlayer);
}

void UGUIS_GameUISubsystem::RemovePlayer(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerDestroyed(LocalPlayer);
}

void UGUIS_GameUISubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UGUIS_GameUISubsystem::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UGUIS_GameUISubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

void UGUIS_GameUISubsystem::RegisterUIActionBinding(UCommonUserWidget* Target, FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback,
                                                    FGUIS_UIActionBindingHandle& BindingHandle)
{
	if (IsValid(Target))
	{
		FBindUIActionArgs BindArgs(InputAction, bShouldDisplayInActionBar, FSimpleDelegate::CreateLambda([InputAction, Callback]()
		{
			Callback.ExecuteIfBound(InputAction.RowName);
		}));

		BindingHandle.Handle = Target->RegisterUIActionBinding(BindArgs);
		BindingHandles.Add(BindingHandle.Handle);
	}
}

void UGUIS_GameUISubsystem::UnregisterBinding(FGUIS_UIActionBindingHandle& BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		UE_LOG(LogGUIS, Display, TEXT("Unregister binding for %s"), *BindingHandle.Handle.GetDisplayName().ToString())

		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
	}
}

void UGUIS_GameUISubsystem::RegisterUIContextForPlayer(ULocalPlayer* LocalPlayer, UGUIS_GameUIContext* Context, FGUIS_UIContextBindingHandle& BindingHandle)
{
	if (LocalPlayer && CurrentPolicy && Context)
	{
		if (CurrentPolicy->AddContext(LocalPlayer, Context))
		{
			BindingHandle = FGUIS_UIContextBindingHandle(LocalPlayer, Context->GetClass());
		}
	}
}

void UGUIS_GameUISubsystem::RegisterUIContextForActor(AActor* Actor, UGUIS_GameUIContext* Context, FGUIS_UIContextBindingHandle& BindingHandle)
{
	if (!IsValid(Actor))
	{
		UE_LOG(LogGUIS, Error, TEXT("Trying to register ui context for invalid pawn!"))
		return;
	}
	APawn* Pawn = Cast<APawn>(Actor);
	if (Pawn == nullptr || !Pawn->IsLocallyControlled())
	{
		UE_LOG(LogGUIS, Error, TEXT("Trying to register ui context for actor(%s) which is not locally controlled pawn!"), *Pawn->GetName())
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (PC == nullptr)
	{
		UE_LOG(LogGUIS, Error, TEXT("Trying to register ui context for pawn(%s) which doesn't have valid player controller"), *Pawn->GetName())
		return;
	}
	RegisterUIContextForPlayer(PC->GetLocalPlayer(), Context, BindingHandle);
}

bool UGUIS_GameUISubsystem::FindUIContextForPlayer(ULocalPlayer* LocalPlayer, TSubclassOf<UGUIS_GameUIContext> ContextClass, UGUIS_GameUIContext*& OutContext)
{
	if (LocalPlayer && CurrentPolicy && ContextClass != nullptr)
	{
		if (UGUIS_GameUIContext* Context = CurrentPolicy->GetContext(LocalPlayer, ContextClass))
		{
			if (Context->GetClass() == ContextClass)
			{
				OutContext = Context;
				return true;
			}
		}
	}
	return false;
}

void UGUIS_GameUISubsystem::UnregisterUIContextForPlayer(FGUIS_UIContextBindingHandle& BindingHandle)
{
	if (BindingHandle.LocalPlayer && BindingHandle.ContextClass && CurrentPolicy)
	{
		CurrentPolicy->RemoveContext(BindingHandle.LocalPlayer, BindingHandle.ContextClass);
		BindingHandle.LocalPlayer = nullptr;
		BindingHandle.ContextClass = nullptr;
	}
}

void UGUIS_GameUISubsystem::SwitchToPolicy(UGUIS_GameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}
