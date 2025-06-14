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


USTRUCT(BlueprintType)
struct GENERICUISYSTEM_API FGUIS_TabDescriptor
{
	GENERATED_BODY()

	FGUIS_TabDescriptor()
		: bHidden(false)
		  , CreatedTabContentWidget(nullptr)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition")
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition")
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition")
	FSlateBrush IconBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition", Transient)
	bool bHidden;

	/**
	 * A common button which implements GUIS_TabButtonInterface to received Label infomation.
	 * 指定用作Tab按钮的Widget类型，该类型必须实现GUIS_TabButtonInterface以接收Label信息。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition", meta = (MustImplement = "/Script/GenericUISystem.GUIS_TabButtonInterface", AllowAbstract = "false"))
	TSoftClassPtr<UCommonButtonBase> TabButtonType;

	/**
	 * The content this tab represents.
	 *  该Tab所呈现的Widget（可选），如果有指定.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tab Definition")
	TSoftClassPtr<UCommonUserWidget> TabContentType;

	UPROPERTY(Transient)
	TObjectPtr<UWidget> CreatedTabContentWidget;
};


UINTERFACE(BlueprintType)
class GENERICUISYSTEM_API UGUIS_TabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 选项卡按钮接口，任何用于切换到不同选项卡的“按钮”都应该实现此接口，实现此接口的按钮，可以得知有关于选项卡的信息，并作出视觉上的更新。
 */
class GENERICUISYSTEM_API IGUIS_TabButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button", meta=(DeprecatedFunction, DeprecationMessage="Use Set TabLabelInfo"))
	void SetTabDefinition(const UGUIS_TabDefinition* TabDefinition);

	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabLabelInfo(const FGUIS_TabDescriptor& TabDescriptor);
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

	/**
	 * get a default registered tab definition by name.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|TabList")
	bool GetPreregisteredTabInfo(const FName TabNameId, FGUIS_TabDescriptor& OutTabInfo);

	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList")
	int32 GetPreregisteredTabIndex(FName TabNameId) const;

	/**
	 * Find a preregistered tab definition.
	 */
	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList", meta=(ExpandBoolAsExecs="ReturnValue"))
	bool FindPreregisteredTabInfo(const FName TabNameId, FGUIS_TabDescriptor& OutTabInfo);

	/**
	 * Toggles whether a specified tab is hidden, can only be called before the switcher is associated。
	 * 让指定的选项卡进行视觉上的开关，只能在关联Switcher之前调用。
	 * @param TabNameId 哪一个选项卡Id
	 * @param bHidden 是否隐藏？
	 */
	UFUNCTION(BlueprintCallable, Category = "GUIS|TabList")
	void SetTabHiddenState(FName TabNameId, bool bHidden);

	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool RegisterDynamicTab(const FGUIS_TabDescriptor& TabDescriptor);

	/**
	 * @return Is the first tab active? 是否第一个选项卡激活？
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool IsFirstTabActive() const;

	/**
	 * @return Is the last tab active? 是否最后一个选项卡激活？
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS|TabList")
	bool IsLastTabActive() const;

	/**
	 * @param TabId Which tab?;哪一个选项卡？
	 * @return If The specified tab visible? 指定的选项卡是否可见？
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GUIS|TabList")
	bool IsTabVisible(FName TabId);

	/**
	 *
	 * @return Return nums of visible tabs. 返回可见选项卡数量。
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

	UPROPERTY(Instanced, meta = (BlueprintProtected, TitleProperty = "TabId"))
	TArray<TObjectPtr<UGUIS_TabDefinition>> TabDefinitions_DEPRECATED;

	UPROPERTY(EditAnywhere, Category="TabList", meta=(TitleProperty="TabId"))
	TArray<FGUIS_TabDescriptor> PreregisteredTabInfoArray;

	/**
	 * Stores label info for tabs that have been registered at runtime but not yet created.
	 * Elements are removed once they are created.
	 */
	UPROPERTY()
	TMap<FName, FGUIS_TabDescriptor> PendingTabLabelInfoMap;

#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
};
