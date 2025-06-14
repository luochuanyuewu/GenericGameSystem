// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Input/UIActionBindingHandle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include "GUIS_GameUISubsystem.generated.h"

class UCommonUserWidget;
class UGUIS_GameUIContext;
class FSubsystemCollectionBase;
class ULocalPlayer;
class UGUIS_GameUIPolicy;
class UObject;

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


/**
 * This manager is intended to be replaced by whatever your game needs to
 * actually create.
 *
 * If you just need the basic functionality you will start by sublcassing this
 * subsystem in your own game.
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_GameUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGUIS_GameUISubsystem()
	{
	}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS")
	const UGUIS_GameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }

	UGUIS_GameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	/**
	 * Intend to be call when player join game.
	 * @param LocalPlayer
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS")
	virtual void AddPlayer(ULocalPlayer* LocalPlayer);

	/**
	 * Intend to be call when player leave game.
	 * @param LocalPlayer
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS")
	virtual void RemovePlayer(ULocalPlayer* LocalPlayer);

	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGUIS_UIActionExecutedDelegate, FName, ActionName);

	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="Target"))
	void RegisterUIActionBinding(UCommonUserWidget* Target, FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback,
	                             FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterBinding(UPARAM(ref)
		FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="LocalPlayer"))
	void RegisterUIContextForPlayer(ULocalPlayer* LocalPlayer, UGUIS_GameUIContext* Context, FGUIS_UIContextBindingHandle& BindingHandle);

	/**
	 * Shortcut method to register ui context for Actor.
	 * @attention The actor must be a locally controlled pawn processed by a player controller!
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="Actor"))
	void RegisterUIContextForActor(AActor* Actor, UGUIS_GameUIContext* Context, FGUIS_UIContextBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="LocalPlayer", DeterminesOutputType="ContextClass", DynamicOutputParam="OutContext", ExpandBoolAsExecs="ReturnValue"))
	bool FindUIContextForPlayer(ULocalPlayer* LocalPlayer, TSubclassOf<UGUIS_GameUIContext> ContextClass, UGUIS_GameUIContext*& OutContext);

	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="LocalPlayer"))
	void UnregisterUIContextForPlayer(UPARAM(ref)
		FGUIS_UIContextBindingHandle& BindingHandle);

protected:
	void SwitchToPolicy(UGUIS_GameUIPolicy* InPolicy);

private:
	UPROPERTY(Transient)
	TObjectPtr<UGUIS_GameUIPolicy> CurrentPolicy = nullptr;

	// TODO, maybe move to each local player's layout info?
	TArray<FUIActionBindingHandle> BindingHandles;
};
