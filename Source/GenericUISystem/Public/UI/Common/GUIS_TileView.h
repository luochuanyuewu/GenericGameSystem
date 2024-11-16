// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTileView.h"
#include "GUIS_TileView.generated.h"

class UGUIS_WidgetFactory;

/**
 * Extended TileView allow dynamic select "Entry Widget Class" via DataAsset.
 * 经过拓展的TileView，允许你通过数据资产动态地选择“Entry Widget Class”。
 */
UCLASS(Blueprintable, meta = (Category = "Generic UI System"))
class GENERICUISYSTEM_API UGUIS_TileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	UGUIS_TileView(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog &InCompileLog) const override;
#endif

	UFUNCTION(BlueprintCallable, Category = "ListEntries")
	void SetEntryWidgetFactories(TArray<UGUIS_WidgetFactory *> NewFactories);

protected:
	virtual UUserWidget &OnGenerateEntryWidgetInternal(UObject *Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase> &OwnerTable) override;
	// virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

protected:
	/**
	 * EntryWidgetFactories allows you dynamically select Entry Widget Class based on your own rules.
	 * EntryWidgetFactories 允许你根据你自己的规则动态地选择Entry Widget Class。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ListEntries", meta = (IsBindableEvent = "True"))
	TArray<TObjectPtr<UGUIS_WidgetFactory>> EntryWidgetFactories;
};
