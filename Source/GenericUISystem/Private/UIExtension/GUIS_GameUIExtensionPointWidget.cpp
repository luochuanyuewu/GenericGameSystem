// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#include "UIExtension/GUIS_GameUIExtensionPointWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor/WidgetCompilerLog.h"
#include "Misc/UObjectToken.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GUIS_GameUIExtensionPointWidget)

#define LOCTEXT_NAMESPACE "UIExtension"

/////////////////////////////////////////////////////
// UGUIS_GameUIExtensionPointWidget

UGUIS_GameUIExtensionPointWidget::UGUIS_GameUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGUIS_GameUIExtensionPointWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	ResetExtensionPoint();

	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UGUIS_GameUIExtensionPointWidget::RebuildWidget()
{
	if (!IsDesignTime() && ExtensionPointTag.IsValid())
	{
		ResetExtensionPoint();
		RegisterExtensionPoint();

		RegisterForPlayerStateIfReady();
	}

	if (IsDesignTime())
	{
		auto GetExtensionPointText = [this]()
		{
			return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "Extension Point\n{0}"), FText::FromName(ExtensionPointTag.GetTagName()));
		};

		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);

		MessageBox->AddSlot()
		          .Padding(5.0f)
		          .HAlign(HAlign_Center)
		          .VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.Text_Lambda(GetExtensionPointText)
		];

		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}

void UGUIS_GameUIExtensionPointWidget::RegisterForPlayerStateIfReady()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
		{
			RegisterExtensionPointForPlayerState(GetOwningLocalPlayer(), PS);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::OnCheckPlayerState, 0.2f);
	}
}

bool UGUIS_GameUIExtensionPointWidget::CheckPlayerState()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
		{
			if (TimerHandle.IsValid())
			{
				TimerHandle.Invalidate();
			}
			RegisterExtensionPointForPlayerState(GetOwningLocalPlayer(), PS);
			return true;
		}
	}

	return false;
}

void UGUIS_GameUIExtensionPointWidget::OnCheckPlayerState()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
		{
			if (TimerHandle.IsValid())
				TimerHandle.Invalidate();
			RegisterExtensionPointForPlayerState(GetOwningLocalPlayer(), PS);
		}
	}
}

void UGUIS_GameUIExtensionPointWidget::ResetExtensionPoint()
{
	ResetInternal();

	ExtensionMapping.Reset();
	for (FGUIS_GameUIExtPointHandle& Handle : ExtensionPointHandles)
	{
		Handle.Unregister();
	}
	ExtensionPointHandles.Reset();
}

void UGUIS_GameUIExtensionPointWidget::RegisterExtensionPoint()
{
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPoint(
			ExtensionPointTag, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, GetOwningLocalPlayer(), ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
	}
}

void UGUIS_GameUIExtensionPointWidget::RegisterExtensionPointForPlayerState(ULocalPlayer* LocalPlayer, APlayerState* PlayerState)
{
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, PlayerState, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
	}
}

void UGUIS_GameUIExtensionPointWidget::OnAddOrRemoveExtension(EGUIS_GameUIExtAction Action, const FGUIS_GameUIExtRequest& Request)
{
	if (Action == EGUIS_GameUIExtAction::Added)
	{
		UObject* Data = Request.Data;

		TSubclassOf<UUserWidget> WidgetClass(Cast<UClass>(Data));
		if (WidgetClass)
		{
			UUserWidget* Widget = CreateEntryInternal(WidgetClass);
			ExtensionMapping.Add(Request.ExtensionHandle, Widget);
		}
		else if (DataClasses.Num() > 0)
		{
			if (GetWidgetClassForData.IsBound())
			{
				WidgetClass = GetWidgetClassForData.Execute(Data);

				// If the data is irrelevant they can just return no widget class.
				if (WidgetClass)
				{
					if (UUserWidget* Widget = CreateEntryInternal(WidgetClass))
					{
						ExtensionMapping.Add(Request.ExtensionHandle, Widget);
						ConfigureWidgetForData.ExecuteIfBound(Widget, Data);
					}
				}
			}
		}
	}
	else
	{
		if (UUserWidget* Extension = ExtensionMapping.FindRef(Request.ExtensionHandle))
		{
			RemoveEntryInternal(Extension);
			ExtensionMapping.Remove(Request.ExtensionHandle);
		}
	}
}

#if WITH_EDITOR
void UGUIS_GameUIExtensionPointWidget::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	// We don't care if the CDO doesn't have a specific tag.
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (!ExtensionPointTag.IsValid())
		{
			TSharedRef<FTokenizedMessage> Message = CompileLog.Error(FText::Format(
				LOCTEXT("UGUIS_GameUIExtensionPointWidget_NoTag", "{0} has no ExtensionPointTag specified - All extension points must specify a tag so they can be located."), FText::FromString(GetName())));
			Message->AddToken(FUObjectToken::Create(this));
		}
	}
}
#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
