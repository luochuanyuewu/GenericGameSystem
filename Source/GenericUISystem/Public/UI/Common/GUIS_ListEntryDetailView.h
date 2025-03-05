// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "GUIS_ListEntryDetailView.generated.h"

class UGUIS_DetailSectionsBuilder;
class UGUIS_ListEntryDetailSection;
class UVerticalBox;
class UGUIS_WidgetFactory;
struct FStreamableHandle;

/**
 * A detail view which contains many detail sections to represent an item(any UObjects.).
 * 
 * 一个细节视图包含多个细节Section来展示一个项（可以是任意UObject类型.）
 */
UCLASS(Abstract, meta=(Category = "Generic UI"))
class GENERICUISYSTEM_API UGUIS_ListEntryDetailView : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UGUIS_ListEntryDetailView(const FObjectInitializer& ObjectInitializer);

	/**
	 * 设置此细节视图所呈现的对象，作为数据。
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS")
	void SetListItemObject(UObject* InListItemObject);

	/**
	 * 设置关联的细节部分构建器。
	 */
	UFUNCTION(BlueprintCallable, Category="GUIS")
	void SetSectionsBuilder(UGUIS_DetailSectionsBuilder* NewBuilder);

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void CreateDetailsExtension(UObject* InData, TSubclassOf<UGUIS_ListEntryDetailSection> SectionClass);

	/**
		 * 细节部分构建器，会根据此数据资产中指定的Widget来展示所呈现的数据。
		 */
	UPROPERTY(EditAnywhere, Category="GUIS", meta = (AllowAbstract = false))
	TObjectPtr<UGUIS_DetailSectionsBuilder> SectionsBuilder;

	UPROPERTY(Transient)
	FUserWidgetPool ExtensionWidgetPool;

	UPROPERTY(Transient)
	TObjectPtr<UObject> CurrentListItemObject;

	TSharedPtr<FStreamableHandle> StreamingHandle;

private: // Bound Widgets
	UPROPERTY(BlueprintReadOnly, Category="GUIS", meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UVerticalBox> Box_DetailSections;
};
