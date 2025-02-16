// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Common/GUIS_ListEntry.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "GUIS_GameUISubsystem.generated.h"

class FSubsystemCollectionBase;
class ULocalPlayer;
class UGUIS_GameUIPolicy;
class UObject;

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


	UFUNCTION(BlueprintCallable, Category="GUIS", meta=(DefaultToSelf="Target"))
	void RegisterUIActionBinding(UCommonUserWidget* Target, FDataTableRowHandle InputAction, bool bShouldDisplayInActionBar, const FGUIS_UIActionExecutedDelegate& Callback,
	                             FGUIS_UIActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
	void UnregisterBinding(UPARAM(ref) FGUIS_UIActionBindingHandle& BindingHandle);

protected:
	void SwitchToPolicy(UGUIS_GameUIPolicy* InPolicy);

private:
	UPROPERTY(Transient)
	TObjectPtr<UGUIS_GameUIPolicy> CurrentPolicy = nullptr;

	TArray<FUIActionBindingHandle> BindingHandles;

#pragma region ModalSystem

#pragma endregion
};
