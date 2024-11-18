// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "GUIS_GameUIExtensionSubsystem.h"
#include "Components/DynamicEntryBoxBase.h"

#include "GUIS_GameUIExtensionPointWidget.generated.h"

class IWidgetCompilerLog;


/**
 * A slot that defines a location in a layout, where content can be added later
 */
UCLASS()
class GENERICUISYSTEM_API UGUIS_GameUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(TSubclassOf<UUserWidget>, FOnGetWidgetClassForData, UObject*, DataItem);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnConfigureWidgetForData, UUserWidget*, Widget, UObject*, DataItem);

	UGUIS_GameUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer);

	//~UWidget interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

	
	void RegisterForPlayerStateIfReady();
	bool CheckPlayerState();

	void OnCheckPlayerState();

	FTimerHandle TimerHandle;
	
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~End of UWidget interface

private:
	void ResetExtensionPoint();
	void RegisterExtensionPoint();
	void RegisterExtensionPointForPlayerState(ULocalPlayer* LocalPlayer, APlayerState* PlayerState);
	void OnAddOrRemoveExtension(EGUIS_GameUIExtAction Action, const FGUIS_GameUIExtRequest& Request);

protected:
	/** The tag that defines this extension point */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	FGameplayTag ExtensionPointTag;

	/** How exactly does the extension need to match the extension point tag. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	EGUIS_GameUIExtPointMatchType ExtensionPointTagMatch = EGUIS_GameUIExtPointMatchType::ExactMatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	TArray<TObjectPtr<UClass>> DataClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Extension", meta=( IsBindableEvent="True" ))
	FOnGetWidgetClassForData GetWidgetClassForData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Extension", meta=( IsBindableEvent="True" ))
	FOnConfigureWidgetForData ConfigureWidgetForData;

	TArray<FGUIS_GameUIExtPointHandle> ExtensionPointHandles;

	UPROPERTY(Transient)
	TMap<FGUIS_GameUIExtHandle, TObjectPtr<UUserWidget>> ExtensionMapping;
};
