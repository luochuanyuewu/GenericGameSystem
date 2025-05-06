// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Styling/SlateBrush.h"
#include "GUIS_TabDefinition.generated.h"

class UCommonActivatableWidget;
class UWidget;
class UCommonUserWidget;
class UCommonButtonBase;

/**
 * Base Tab definition.
 * 基础选项卡定义。
 */
UCLASS(Blueprintable, EditInlineNew, CollapseCategories, Const, DefaultToInstanced)
class GENERICUISYSTEM_API UGUIS_TabDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS")
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GUIS")
	FSlateBrush IconBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS")
	bool bHidden;

	/**
	 * A common button which implements GUIS_TabButtonInterface to received Label infomation.
	 * 指定用作Tab按钮的Widget类型，该类型必须实现GUIS_TabButtonInterface以接收Label信息。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS", meta = (MustImplement = "/Script/GenericUISystem.GUIS_TabButtonInterface", AllowAbstract = "false"))
	TSoftClassPtr<UCommonButtonBase> TabButtonType;

	/**
	 * 该所呈现的Widget（可选），如果有指定，那么需要调用
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GUIS")
	TSoftClassPtr<UCommonActivatableWidget> TabContentType;

	UPROPERTY(Transient)
	TObjectPtr<UWidget> CreatedTabContentWidget;
};
