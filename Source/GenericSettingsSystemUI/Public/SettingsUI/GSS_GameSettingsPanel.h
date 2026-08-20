// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Containers/Ticker.h"
#include "Settings/GSS_GameSettingFilterState.h"

#include "GSS_GameSettingsPanel.generated.h"

class UGSS_GameSetting;
class UGSS_GameSettingCollectionPage;
class UGSS_GameSettingRegistry;
class UGSS_GameSettingDetailView;
class UGUIS_ListView;
struct FFocusEvent;
struct FGeometry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGSS_OnFocusedSettingChanged, UGSS_GameSetting*, Setting);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGSS_OnExecuteSettingNamedAction, UGSS_GameSetting*, Setting, FGameplayTag, Action);

/**
 * CommonUI-facing presenter for one GSS Registry.
 * 一个 GSS Registry 的 CommonUI 展示器。
 *
 * Bind ListView_Settings in a Blueprint subclass and supply entry factories to that list. The panel owns
 * filtering and page navigation; Apply/Cancel/Reset belong on the Menu or Subsystem.
 * 请在蓝图子类中绑定 ListView_Settings，并为该列表配置条目 Factory。Panel 负责筛选与页面导航；
 * Apply/Cancel/Reset 属于 Menu 或 Subsystem。
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick, Category = "Generic Settings UI"))
class GENERICSETTINGSSYSTEMUI_API UGSS_GameSettingsPanel : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UGSS_GameSettingsPanel();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	/** Assigns the Registry this panel displays. / 指定此 Panel 要展示的 Registry。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void SetRegistry(UGSS_GameSettingRegistry* InRegistry);

	/** Replaces the active list query. By default this starts a new navigation branch. / 替换当前列表查询；默认会开始一条新的导航分支。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void SetFilterState(const FGSS_GameSettingFilterState& InFilterState, bool bClearNavigationStack = true);
	/** Returns the complete query currently driving the list. / 返回当前驱动列表的完整查询条件。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	FGSS_GameSettingFilterState GetFilterState() const { return FilterState; }

	/** Shows the requested page and pushes the complete previous query onto the navigation stack. / 展示请求页面，并将完整的前一查询压入导航栈。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	bool NavigateToPage(UGSS_GameSettingCollectionPage* Page);
	/** Returns whether a previous complete query is available. / 返回是否存在可恢复的前一完整查询。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	bool CanPopNavigationStack() const;
	/** Restores the previous query, if one exists. / 恢复前一查询。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void PopNavigationStack();
	/** Requeries the Registry and refreshes the list. / 重新查询 Registry 并刷新列表。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void RefreshSettingsList();

	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_GameSettingCollectionPage* GetCurrentPage() const;
	/** Returns items displayed by the most recent completed list refresh. / 返回最近一次完成列表刷新后显示的条目。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<UGSS_GameSetting*> GetVisibleSettings() const { return VisibleSettings; }

	/** Selects a visible setting after the next refresh. / 在下一次刷新后选中可见设置。 */
	UFUNCTION(BlueprintCallable, Category = "GSS|Settings UI")
	void SelectSetting(FGameplayTag SettingId);
	/** Returns the currently selected list setting. / 返回当前选中的列表设置。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	UGSS_GameSetting* GetSelectedSetting() const;
	/** Returns potentially available reset targets, including currently hidden or disabled items. / 返回潜在可恢复默认值的目标，包括当前隐藏或禁用的项目。 */
	UFUNCTION(BlueprintPure, Category = "GSS|Settings UI")
	TArray<UGSS_GameSetting*> GetSettingsWeCanResetToDefault() const;

	/** Broadcast when hover or selection changes the focused setting. / 悬停或选择改变焦点设置时广播。 */
	UPROPERTY(BlueprintAssignable, Category = "GSS|Settings UI")
	FGSS_OnFocusedSettingChanged OnFocusedSettingChanged;
	/** Broadcast when a named action node executes. Push an editor widget that implements IGSS_GameSettingActionInterface, then call ExecuteActionForSetting. / 命名 Action 节点执行时广播。请推入实现 IGSS_GameSettingActionInterface 的编辑器 Widget，再调用 ExecuteActionForSetting。 */
	UPROPERTY(BlueprintAssignable, Category = Events, meta = (DisplayName = "On Execute Named Action"))
	FGSS_OnExecuteSettingNamedAction BP_OnExecuteNamedAction;

protected:
	void HandleNavigationRequested(UGSS_GameSetting* Setting);
	void HandleNamedAction(UGSS_GameSetting* Setting, FGameplayTag ActionTag);
	void HandleEditableStateChanged(UGSS_GameSetting* Setting);
	void HandleSettingItemHoveredChanged(UObject* Item, bool bHovered);
	void HandleSettingItemSelectionChanged(UObject* Item);
	void FillSettingDetails(UGSS_GameSetting* Setting);
	void BindRegistryEvents();
	void UnbindRegistryEvents();

	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSettingRegistry> Registry;
	UPROPERTY(Transient)
	FGSS_GameSettingFilterState FilterState;
	UPROPERTY(Transient)
	TArray<FGSS_GameSettingFilterState> FilterNavigationStack;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> VisibleSettings;
	UPROPERTY(Transient)
	TObjectPtr<UGSS_GameSetting> LastHoveredOrSelectedSetting;
	FGameplayTag DesiredSelectionSettingId;
	bool bAdjustListViewPostRefresh = true;
	FTSTicker::FDelegateHandle RefreshHandle;

	/** List receiving queried runtime setting nodes. / 接收查询到的运行时设置节点的列表。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UGUIS_ListView> ListView_Settings;

	/** Optional standard GSS detail view receiving the hovered or selected runtime setting. / 可选的标准 GSS 详情视图，接收悬停或选中的运行时设置。 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true), Category = "GSS|Settings UI")
	TObjectPtr<UGSS_GameSettingDetailView> Details_Settings;
};
