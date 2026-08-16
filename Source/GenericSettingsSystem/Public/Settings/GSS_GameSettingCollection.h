// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GSS_GameSetting.h"

#include "GSS_GameSettingCollection.generated.h"

struct FGSS_GameSettingFilterState;

//--------------------------------------
// UGSS_GameSettingCollection
//--------------------------------------

/** Non-selectable container that owns and filters child setting nodes. / 持有并筛选子设置节点的不可选择容器。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingCollection : public UGSS_GameSetting
{
	GENERATED_BODY()

public:
	UGSS_GameSettingCollection();

	/** Returns direct children in display order. / 按显示顺序返回直接子节点。 */
	virtual TArray<UGSS_GameSetting*> GetChildSettings() override { return Settings; }
	/** Returns direct children that are collections. / 返回直接子节点中的 Collection。 */
	TArray<UGSS_GameSettingCollection*> GetChildCollections() const;

	/** Adds and initializes a child for this collection's LocalPlayer. / 添加子节点并为该 Collection 的 LocalPlayer 初始化。 */
	void AddSetting(UGSS_GameSetting* Setting);
	/** Detaches a child without destroying it. / 分离子节点但不销毁它。 */
	void RemoveSetting(UGSS_GameSetting* Setting);
	/** Appends recursively visible/filterable descendants for settings UI. / 为设置 UI 追加递归可见且可筛选的后代。 */
	virtual void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const;

	virtual bool IsSelectable() const { return false; }

protected:
	/** Runtime children owned by this collection. / 此 Collection 持有的运行时子节点。 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UGSS_GameSetting>> Settings;

};

//--------------------------------------
// UGSS_GameSettingCollectionPage
//--------------------------------------

/** Selectable collection representing a separately navigated settings page. / 代表单独导航设置页面的可选择 Collection。 */
UCLASS()
class GENERICSETTINGSSYSTEM_API UGSS_GameSettingCollectionPage : public UGSS_GameSettingCollection
{
	GENERATED_BODY()

public:
	/** Native event requesting UI navigation into this page. / 请求 UI 导航进入此页面的原生事件。 */
	DECLARE_EVENT_OneParam(UGSS_GameSettingCollectionPage, FOnExecuteNavigation, UGSS_GameSetting* /*Setting*/);

	FOnExecuteNavigation OnExecuteNavigationEvent;

public:
	UGSS_GameSettingCollectionPage();

	/** Returns the localized navigation label. / 返回本地化导航标签。 */
	FText GetNavigationText() const { return NavigationText; }
	void SetNavigationText(FText Value) { NavigationText = Value; }
#if !UE_BUILD_SHIPPING
	void SetNavigationText(const FString& Value) { SetNavigationText(FText::FromString(Value)); }
#endif

	virtual void OnInitialized() override;
	virtual void GetSettingsForFilter(const FGSS_GameSettingFilterState& FilterState, TArray<UGSS_GameSetting*>& InOutSettings) const override;
	virtual bool IsSelectable() const override { return true; }

	/** Emits the page navigation request. / 发出页面导航请求。 */
	void ExecuteNavigation();

private:
	FText NavigationText;
};
