// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonTabListWidgetBase.h"

#include "GUIS_TabListWidgetBase.generated.h"

class UGUIS_TabDefinition;
class UCommonButtonBase;
class UCommonUserWidget;
class UObject;
class UWidget;
struct FFrame;

UINTERFACE(BlueprintType)
class UGUIS_TabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 选项卡按钮接口，任何用于切换到不同选项卡的“按钮”都应该实现此接口，实现此接口的按钮，可以得知有关于选项卡的信息，并作出视觉上的更新。
 */
class IGUIS_TabButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabDefinition(const UGUIS_TabDefinition* TabDefinition);
};

/**
 * 这是一个包含多个选项卡按钮的列表组件，用于显示和管理选项卡。
 * 可以关联一个Switcher（通过SetLinkedSwitcher）以实现动态选项卡切换。
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisableNativeTick))
class GENERICUISYSTEM_API UGUIS_TabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	UGUIS_TabListWidgetBase();

	// UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|TabList")
	// bool GetPreregisteredTabInfo(const FName TabNameId, FGUIS_TabDescriptor& OutTabInfo);

	/**
	 * get a default registered tab definition by name.
	 */
	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList")
	const UGUIS_TabDefinition* GetTabDefinition(FName TabNameId) const;

	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList")
	int32 GetTabDefinitionIndex(FName TabNameId) const;

	/**
	 * Find a typed default registered tab definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList", meta=(DeterminesOutputType="DesiredClass", DynamicOutputParam="ReturnValue"))
	const UGUIS_TabDefinition* FindTabDefinition(const FName TabNameId, TSubclassOf<UGUIS_TabDefinition> DesiredClass);

	/** Helper method to get at all the preregistered tab infos */
	TArray<TObjectPtr<UGUIS_TabDefinition>> GetAllTabDefinitions() { return TabDefinitions; }

	/**
	 * Toggles whether a specified tab is hidden, can only be called before the switcher is associated。
	 * 让指定的选项卡进行视觉上的开关，只能在关联Switcher之前调用。
	 * @param TabNameId 哪一个选项卡Id
	 * @param bHidden 是否隐藏？
	 */
	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList")
	void SetTabHiddenState(FName TabNameId, bool bHidden);

	/**
	 * 给定一个选项卡信息，动态地添加一个选项卡。
	 * @param TabDefinition 选项卡的信息
	 * @return 是否成功注册？
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool RegisterDynamicTab(const UGUIS_TabDefinition* TabDefinition);

	/**
	 * @return 是否第一个选项卡激活？
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool IsFirstTabActive() const;

	/**
	 * @return 是否最后一个选项卡激活？
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool IsLastTabActive() const;

	/**
	 * @param TabId 哪一个选项卡？
	 * @return 指定的选项卡是否可见？
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|TabList")
	bool IsTabVisible(FName TabId);

	/**
	 *
	 * @return 返回可见选项卡数量。
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|TabList")
	int32 GetVisibleTabCount();

	/** Delegate broadcast when a new tab is created. Allows hook ups after creation. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabContentCreated, FName, TabId, UCommonUserWidget *, TabWidget);

	DECLARE_EVENT_TwoParams(UGUIS_TabListWidgetBase, FOnTabContentCreatedNative, FName /* TabId */, UCommonUserWidget * /* TabWidget */);

	/**
	 * Broadcasts when a new tab is created.
	 * 该事件会在一个新的选项卡被创建时进行广播。
	 */
	UPROPERTY(BlueprintAssignable, Category="GUIS|TabList")
	FOnTabContentCreated OnTabContentCreated;
	FOnTabContentCreatedNative OnTabContentCreatedNative;

protected:
	// UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget

	virtual void HandlePreLinkedSwitcherChanged() override;
	virtual void HandlePostLinkedSwitcherChanged() override;

	virtual void HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton) override;

	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	void SetupTabs();

	UPROPERTY(EditAnywhere, Category="TabList", Instanced, meta = (BlueprintProtected, TitleProperty = "TabId"))
	TArray<TObjectPtr<UGUIS_TabDefinition>> TabDefinitions;

	/**
	 * Stores label info for tabs that have been registered at runtime but not yet created.
	 * Elements are removed once they are created.
	 */
	UPROPERTY()
	TMap<FName, TObjectPtr<const UGUIS_TabDefinition>> PendingTabLabelInfoMap;

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
};
