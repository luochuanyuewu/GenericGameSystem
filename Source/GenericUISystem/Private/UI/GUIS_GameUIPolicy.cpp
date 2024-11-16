// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#include "UI/GUIS_GameUIPolicy.h"
#include "UI/GUIS_GameUISubsystem.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"
#include "GUIS_LogChannels.h"
#include "UI/GUIS_GameUILayout.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_GameUIPolicy)

// Static
UGUIS_GameUIPolicy* UGUIS_GameUIPolicy::GetGameUIPolicy(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UGUIS_GameUISubsystem* UIManager = UGameInstance::GetSubsystem<UGUIS_GameUISubsystem>(GameInstance))
			{
				return UIManager->GetCurrentUIPolicy();
			}
		}
	}

	return nullptr;
}

UGUIS_GameUISubsystem* UGUIS_GameUIPolicy::GetOwningUIManager() const
{
	return CastChecked<UGUIS_GameUISubsystem>(GetOuter());
}

UWorld* UGUIS_GameUIPolicy::GetWorld() const
{
	if (GetOwningUIManager())
	{
		return GetOwningUIManager()->GetGameInstance()->GetWorld();
	}
	return nullptr;
}

UGUIS_GameUILayout* UGUIS_GameUIPolicy::GetRootLayout(const ULocalPlayer* LocalPlayer) const
{
	const FGUIS_RootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

void UGUIS_GameUIPolicy::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	if (FGUIS_RootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGUIS_GameUIPolicy::NotifyPlayerRemoved(ULocalPlayer* LocalPlayer)
{
	if (FGUIS_RootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = false;

		if (LocalMultiplayerInteractionMode == EGUIS_LocalMultiplayerInteractionMode::SingleToggle && !LocalPlayer->IsPrimaryPlayer())
		{
			UGUIS_GameUILayout* RootLayout = LayoutInfo->RootLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				// We're removing a secondary player's root while it's in control - transfer control back to the primary player's root
				RootLayout->SetIsDormant(true);
				for (const FGUIS_RootViewportLayoutInfo& RootLayoutInfo : RootViewportLayouts)
				{
					if (RootLayoutInfo.LocalPlayer->IsPrimaryPlayer())
					{
						if (UGUIS_GameUILayout* PrimaryRootLayout = RootLayoutInfo.RootLayout)
						{
							PrimaryRootLayout->SetIsDormant(false);
						}
					}
				}
			}
		}
	}
}

void UGUIS_GameUIPolicy::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UGUIS_GameUILayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(LocalPlayer, Layout);

		OnRootLayoutReleased(LocalPlayer, Layout);
	}
}

void UGUIS_GameUIPolicy::AddLayoutToViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout)
{
	UE_LOG(LogGUIS, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UGUIS_GameUIPolicy::RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogGUIS, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogGUIS, Log, TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."),
			       *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		}

		OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
	}
}

void UGUIS_GameUIPolicy::OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
	BP_OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UGUIS_GameUIPolicy::OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout)
{
	BP_OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
}

void UGUIS_GameUIPolicy::OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout)
{
	BP_OnRootLayoutReleased(LocalPlayer, Layout);
}

void UGUIS_GameUIPolicy::RequestPrimaryControl(UGUIS_GameUILayout* Layout)
{
	if (LocalMultiplayerInteractionMode == EGUIS_LocalMultiplayerInteractionMode::SingleToggle && Layout->IsDormant())
	{
		for (const FGUIS_RootViewportLayoutInfo& LayoutInfo : RootViewportLayouts)
		{
			UGUIS_GameUILayout* RootLayout = LayoutInfo.RootLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				RootLayout->SetIsDormant(true);
				break;
			}
		}
		Layout->SetIsDormant(false);
	}
}

void UGUIS_GameUIPolicy::CreateLayoutWidget(ULocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UGUIS_GameUILayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UGUIS_GameUILayout* NewLayoutObject = CreateWidget<UGUIS_GameUILayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UGUIS_GameUILayout> UGUIS_GameUIPolicy::GetLayoutWidgetClass(ULocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}
