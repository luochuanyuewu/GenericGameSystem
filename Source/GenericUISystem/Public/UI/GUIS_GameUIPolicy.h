// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "GUIS_GameUIPolicy.generated.h"

class UGUIS_GameUIContext;
class ULocalPlayer;
class UGUIS_GameUISubsystem;
class ULocalPlayer;
class UGUIS_GameUILayout;

/**
 *
 */
UENUM()
enum class EGUIS_LocalMultiplayerInteractionMode : uint8
{
	// Fullscreen viewport for the primary player only, regardless of the other player's existence
	PrimaryOnly,

	// Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
	SingleToggle,

	// Viewports displayed simultaneously for both players
	Simultaneous
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

	FGUIS_RootViewportLayoutInfo()
	{
	}

	FGUIS_RootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UGUIS_GameUILayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bAddedToViewport(bIsInViewport)
	{
	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

/**
 * UI policy manages each game ui layout for different local players.
 * UI 策略管理每一个本地玩家的UI布局。
 */
UCLASS(Abstract, Blueprintable, Within = GUIS_GameUISubsystem)
class GENERICUISYSTEM_API UGUIS_GameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename GameUIPolicyClass = UGUIS_GameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<GameUIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UGUIS_GameUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);

	virtual UWorld* GetWorld() const override;
	UGUIS_GameUISubsystem* GetOwningSubsystem() const;

	UGUIS_GameUILayout* GetRootLayout(const ULocalPlayer* LocalPlayer) const;

	virtual UGUIS_GameUIContext* GetContext(const ULocalPlayer* LocalPlayer, TSubclassOf<UGUIS_GameUIContext> ContextClass);
	virtual bool AddContext(const ULocalPlayer* LocalPlayer, UGUIS_GameUIContext* NewContext);
	virtual void RemoveContext(const ULocalPlayer* LocalPlayer, TSubclassOf<UGUIS_GameUIContext> ContextClass);

	EGUIS_LocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	void RequestPrimaryControl(UGUIS_GameUILayout* Layout);

protected:
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	virtual void OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnRootLayoutAddedToViewport"))
	void BP_OnRootLayoutAddedToViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	virtual void OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnRootLayoutRemovedFromViewport"))
	void BP_OnRootLayoutRemovedFromViewport(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	virtual void OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	/**
	 *
	 * @param LocalPlayer
	 * @param Layout
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnRootLayoutReleased"))
	void BP_OnRootLayoutReleased(ULocalPlayer* LocalPlayer, UGUIS_GameUILayout* Layout);

	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	TSubclassOf<UGUIS_GameUILayout> GetLayoutWidgetClass(ULocalPlayer* LocalPlayer);

private:
	EGUIS_LocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = EGUIS_LocalMultiplayerInteractionMode::PrimaryOnly;

	/**
	 * The widget will be used as GameUILayout. 此widget用于游戏UI布局。
	 */
	UPROPERTY(EditAnywhere, Category="GUIS")
	TSoftClassPtr<UGUIS_GameUILayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FGUIS_RootViewportLayoutInfo> RootViewportLayouts;

	void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	friend class UGUIS_GameUISubsystem;
};
