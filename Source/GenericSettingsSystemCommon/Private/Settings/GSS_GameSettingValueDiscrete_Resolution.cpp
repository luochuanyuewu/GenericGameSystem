// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Resolution.h"

#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/GameUserSettings.h"
#include "Settings/GSS_GameSettingFilterState.h"
#include "UnrealEngine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Resolution)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscrete_Resolution"

FString UGSS_GameSettingValueDiscrete_Resolution::ToResolutionValue(const FIntPoint& Resolution)
{
	return FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
}

void UGSS_GameSettingValueDiscrete_Resolution::BeginDestroy()
{
	UnbindDisplayListeners();
	Super::BeginDestroy();
}

void UGSS_GameSettingValueDiscrete_Resolution::OnInitialized()
{
	Super::OnInitialized();
	BindDisplayListeners();
	InitializeResolutions();
}

void UGSS_GameSettingValueDiscrete_Resolution::StoreInitial()
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		InitialValue = ToResolutionValue(Settings->GetScreenResolution());
	}
}

void UGSS_GameSettingValueDiscrete_Resolution::ResetToDefault()
{
}

void UGSS_GameSettingValueDiscrete_Resolution::RestoreToInitial()
{
	FString WidthString;
	FString HeightString;
	if (InitialValue.Split(TEXT("x"), &WidthString, &HeightString, ESearchCase::IgnoreCase))
	{
		const FIntPoint Resolution(FCString::Atoi(*WidthString), FCString::Atoi(*HeightString));
		if (Resolution.X > 0 && Resolution.Y > 0)
		{
			SetResolution(Resolution, EGSS_GameSettingChangeReason::RestoreToInitial);
		}
	}
}

void UGSS_GameSettingValueDiscrete_Resolution::SetDiscreteOptionByIndex(int32 Index)
{
	const TArrayView<const TSharedPtr<FScreenResolutionEntry>> Resolutions = GetSelectedResolutionList();
	if (Resolutions.IsValidIndex(Index) && Resolutions[Index].IsValid())
	{
		SetResolution(Resolutions[Index]->GetResolution(), EGSS_GameSettingChangeReason::Change);
	}
}

int32 UGSS_GameSettingValueDiscrete_Resolution::GetDiscreteOptionIndex() const
{
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		return FindIndexOfDisplayResolution(Settings->GetScreenResolution());
	}
	return INDEX_NONE;
}

int32 UGSS_GameSettingValueDiscrete_Resolution::GetDiscreteOptionDefaultIndex() const
{
	FString WidthString;
	FString HeightString;
	if (InitialValue.Split(TEXT("x"), &WidthString, &HeightString, ESearchCase::IgnoreCase))
	{
		return FindIndexOfDisplayResolution(FIntPoint(FCString::Atoi(*WidthString), FCString::Atoi(*HeightString)));
	}
	return INDEX_NONE;
}

TArray<FText> UGSS_GameSettingValueDiscrete_Resolution::GetDiscreteOptions() const
{
	TArray<FText> Options;
	for (const TSharedPtr<FScreenResolutionEntry>& Entry : GetSelectedResolutionList())
	{
		if (Entry.IsValid())
		{
			Options.Add(Entry->GetDisplayText());
		}
	}
	return Options;
}

void UGSS_GameSettingValueDiscrete_Resolution::OnDependencyChanged()
{
	InitializeResolutions();
	if (const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		const TArrayView<const TSharedPtr<FScreenResolutionEntry>> Resolutions = GetSelectedResolutionList();
		const int32 ClosestIndex = FindClosestResolutionIndex(Settings->GetScreenResolution());
		if (Resolutions.IsValidIndex(ClosestIndex) && Resolutions[ClosestIndex].IsValid())
		{
			SetResolution(Resolutions[ClosestIndex]->GetResolution(), EGSS_GameSettingChangeReason::DependencyChanged);
			return;
		}
	}
	RefreshEditableState();
}

void UGSS_GameSettingValueDiscrete_Resolution::BindDisplayListeners()
{
	if (FSlateApplication::IsInitialized())
	{
		if (const TSharedPtr<GenericApplication> PlatformApplication = FSlateApplication::Get().GetPlatformApplication())
		{
			FDisplayMetrics::RebuildDisplayMetrics(CurrentDisplayMetrics);
			GenericApplication::FOnDisplayMetricsChanged& DisplayMetricsChangedEvent = PlatformApplication->OnDisplayMetricsChanged();
			if (!DisplayMetricsChangedEvent.IsBoundToObject(this))
			{
				DisplayMetricsChangedHandle = DisplayMetricsChangedEvent.AddUObject(this, &ThisClass::OnDisplayMetricsChanged);
			}
		}
	}

	if (const ULocalPlayer* OwningPlayer = GetOwningLocalPlayer())
	{
		if (UGameViewportClient* ViewportClient = OwningPlayer->ViewportClient)
		{
			if (!ViewportClient->OnWindowDisplayChanged().IsBoundToObject(this))
			{
				WindowDisplayChangedHandle = ViewportClient->OnWindowDisplayChanged().AddUObject(this, &ThisClass::HandleExternalDisplayChanged);
			}
			if (!ViewportClient->OnToggleFullscreen().IsBoundToObject(this))
			{
				ToggleFullscreenHandle = ViewportClient->OnToggleFullscreen().AddUObject(this, &ThisClass::HandleToggleFullscreen);
			}
		}
	}

	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		if (!Settings->OnGameUserSettingsVideoRevert.IsBoundToObject(this))
		{
			VideoRevertHandle = Settings->OnGameUserSettingsVideoRevert.AddUObject(this, &ThisClass::HandleExternalDisplayChanged);
		}
	}
}

void UGSS_GameSettingValueDiscrete_Resolution::UnbindDisplayListeners()
{
	if (FSlateApplication::IsInitialized())
	{
		if (const TSharedPtr<GenericApplication> PlatformApplication = FSlateApplication::Get().GetPlatformApplication())
		{
			PlatformApplication->OnDisplayMetricsChanged().Remove(DisplayMetricsChangedHandle);
		}
	}
	DisplayMetricsChangedHandle.Reset();

	if (const ULocalPlayer* OwningPlayer = GetOwningLocalPlayer())
	{
		if (UGameViewportClient* ViewportClient = OwningPlayer->ViewportClient)
		{
			ViewportClient->OnWindowDisplayChanged().Remove(WindowDisplayChangedHandle);
			ViewportClient->OnToggleFullscreen().Remove(ToggleFullscreenHandle);
		}
	}
	WindowDisplayChangedHandle.Reset();
	ToggleFullscreenHandle.Reset();

	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		Settings->OnGameUserSettingsVideoRevert.Remove(VideoRevertHandle);
	}
	VideoRevertHandle.Reset();
}

void UGSS_GameSettingValueDiscrete_Resolution::OnDisplayMetricsChanged(const FDisplayMetrics& NewDisplayMetrics)
{
	CurrentDisplayMetrics = NewDisplayMetrics;
	InitializeResolutions();
	RefreshEditableState();
}

void UGSS_GameSettingValueDiscrete_Resolution::HandleExternalDisplayChanged()
{
	InitializeResolutions();
	RefreshEditableState();
}

void UGSS_GameSettingValueDiscrete_Resolution::HandleToggleFullscreen(bool)
{
	HandleExternalDisplayChanged();
}

void UGSS_GameSettingValueDiscrete_Resolution::SetResolution(const FIntPoint& Resolution, EGSS_GameSettingChangeReason Reason)
{
	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		if (Resolution.X > 0 && Resolution.Y > 0)
		{
			Settings->SetScreenResolution(Resolution);
		}
	}
	NotifySettingChanged(Reason);
}

const FMonitorInfo* UGSS_GameSettingValueDiscrete_Resolution::GetCurrentMonitor() const
{
	const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	if (!Settings)
	{
		return nullptr;
	}

	const int32 MonitorIndex = CurrentDisplayMetrics.GetClosestMonitorFromIDAndIndex(Settings->GetDisplayID(), Settings->GetDisplayIndex());
	return CurrentDisplayMetrics.MonitorInfo.IsValidIndex(MonitorIndex) ? &CurrentDisplayMetrics.MonitorInfo[MonitorIndex] : nullptr;
}

void UGSS_GameSettingValueDiscrete_Resolution::InitializeResolutions()
{
	ResolutionsFullscreen.Reset();
	ResolutionsWindowed.Reset();
	ResolutionsWindowedFullscreen.Reset();

	const FMonitorInfo* const Monitor = GetCurrentMonitor();
	FScreenResolutionArray ResArray;
	if (Monitor)
	{
		RHIGetAvailableResolutionsForDisplay(ResArray, true, Monitor->NativeHandle);
	}
	else
	{
		RHIGetAvailableResolutions(ResArray, true);
	}

	{
		TArray<FIntPoint> WindowedResolutions;
		const FIntPoint MinResolution(1280, 720);
		FIntPoint MaxResolution;
		if (Monitor)
		{
			MaxResolution = FIntPoint(Monitor->WorkArea.Right - Monitor->WorkArea.Left, Monitor->WorkArea.Bottom - Monitor->WorkArea.Top);
		}
		else
		{
			MaxResolution = FIntPoint(
				CurrentDisplayMetrics.PrimaryDisplayWorkAreaRect.Right - CurrentDisplayMetrics.PrimaryDisplayWorkAreaRect.Left,
				CurrentDisplayMetrics.PrimaryDisplayWorkAreaRect.Bottom - CurrentDisplayMetrics.PrimaryDisplayWorkAreaRect.Top);
		}

		const float MinAspectRatio = 16.0f / 10.0f;
		if (MaxResolution.X >= MinResolution.X && MaxResolution.Y >= MinResolution.Y)
		{
			GetStandardWindowResolutions(MinResolution, MaxResolution, MinAspectRatio, WindowedResolutions);
		}

		if (GSystemResolution.WindowMode == EWindowMode::Windowed)
		{
			if (GSystemResolution.ResX <= MaxResolution.X && GSystemResolution.ResY <= MaxResolution.Y)
			{
				WindowedResolutions.AddUnique(FIntPoint(GSystemResolution.ResX, GSystemResolution.ResY));
			}
			WindowedResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) { return A.X != B.X ? A.X < B.X : A.Y < B.Y; });
		}

		if (WindowedResolutions.IsEmpty())
		{
			WindowedResolutions.Add(FIntPoint(CurrentDisplayMetrics.PrimaryDisplayWidth, CurrentDisplayMetrics.PrimaryDisplayHeight));
		}

		ResolutionsWindowed.Reserve(WindowedResolutions.Num());
		for (const FIntPoint& Res : WindowedResolutions)
		{
			TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
			Entry->Width = Res.X;
			Entry->Height = Res.Y;
			ResolutionsWindowed.Add(Entry);
		}
	}

	{
		TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
		if (Monitor)
		{
			const FPlatformRect& DisplayRect = Monitor->DisplayRect;
			Entry->Width = DisplayRect.Right - DisplayRect.Left;
			Entry->Height = DisplayRect.Bottom - DisplayRect.Top;
		}
		else
		{
			Entry->Width = CurrentDisplayMetrics.PrimaryDisplayWidth;
			Entry->Height = CurrentDisplayMetrics.PrimaryDisplayHeight;
		}
		ResolutionsWindowedFullscreen.Add(Entry);
	}

	if (!ResArray.IsEmpty())
	{
		for (int32 FilterThreshold = 0; FilterThreshold < 3; ++FilterThreshold)
		{
			for (const FScreenResolutionRHI& ScreenRes : ResArray)
			{
				if (ShouldAllowFullScreenResolution(ScreenRes, FilterThreshold))
				{
					TSharedRef<FScreenResolutionEntry> Entry = MakeShared<FScreenResolutionEntry>();
					Entry->Width = ScreenRes.Width;
					Entry->Height = ScreenRes.Height;
					Entry->RefreshRate = ScreenRes.RefreshRate;
					ResolutionsFullscreen.Add(Entry);
				}
			}

			if (!ResolutionsFullscreen.IsEmpty())
			{
				break;
			}
		}
	}

	if (ResolutionsFullscreen.IsEmpty() && !ResolutionsWindowedFullscreen.IsEmpty())
	{
		ResolutionsFullscreen.Add(ResolutionsWindowedFullscreen[0]);
	}
}

TArrayView<const TSharedPtr<UGSS_GameSettingValueDiscrete_Resolution::FScreenResolutionEntry>> UGSS_GameSettingValueDiscrete_Resolution::GetSelectedResolutionList() const
{
	const UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	const EWindowMode::Type WindowMode = Settings ? Settings->GetFullscreenMode() : EWindowMode::WindowedFullscreen;
	switch (WindowMode)
	{
	case EWindowMode::Windowed:
		return MakeArrayView(ResolutionsWindowed);
	case EWindowMode::Fullscreen:
		return MakeArrayView(ResolutionsFullscreen);
	default:
		return MakeArrayView(ResolutionsWindowedFullscreen);
	}
}

bool UGSS_GameSettingValueDiscrete_Resolution::ShouldAllowFullScreenResolution(const FScreenResolutionRHI& SrcScreenRes, int32 FilterThreshold) const
{
	FScreenResolutionRHI ScreenRes = SrcScreenRes;
	const bool bIsPortrait = ScreenRes.Width < ScreenRes.Height;
	float AspectRatio = ScreenRes.Height > 0 ? static_cast<float>(ScreenRes.Width) / static_cast<float>(ScreenRes.Height) : 0.0f;
	if (bIsPortrait && AspectRatio > 0.0f)
	{
		AspectRatio = 1.0f / AspectRatio;
		ScreenRes.Width = SrcScreenRes.Height;
		ScreenRes.Height = SrcScreenRes.Width;
	}

	if (FilterThreshold < 1)
	{
		float DisplayAspect = AspectRatio;
		if (const FMonitorInfo* const Monitor = GetCurrentMonitor())
		{
			if (Monitor->NativeHeight > 0)
			{
				DisplayAspect = static_cast<float>(Monitor->NativeWidth) / static_cast<float>(Monitor->NativeHeight);
			}
		}
		else
		{
			for (const FMonitorInfo& MonitorInfo : CurrentDisplayMetrics.MonitorInfo)
			{
				if (MonitorInfo.bIsPrimary && MonitorInfo.NativeHeight > 0)
				{
					DisplayAspect = static_cast<float>(MonitorInfo.NativeWidth) / static_cast<float>(MonitorInfo.NativeHeight);
					break;
				}
			}
		}

		if (FMath::Abs(DisplayAspect - AspectRatio) > KINDA_SMALL_NUMBER)
		{
			return false;
		}
	}

	if (FilterThreshold < 2 && (ScreenRes.Width < 1280 || ScreenRes.Height < 720))
	{
		return false;
	}

	return true;
}

int32 UGSS_GameSettingValueDiscrete_Resolution::FindIndexOfDisplayResolution(const FIntPoint& InPoint) const
{
	const TArrayView<const TSharedPtr<FScreenResolutionEntry>> Resolutions = GetSelectedResolutionList();
	for (int32 Index = 0; Index < Resolutions.Num(); ++Index)
	{
		if (Resolutions[Index].IsValid() && Resolutions[Index]->GetResolution() == InPoint)
		{
			return Index;
		}
	}
	return Resolutions.IsEmpty() ? INDEX_NONE : Resolutions.Num() - 1;
}

int32 UGSS_GameSettingValueDiscrete_Resolution::FindClosestResolutionIndex(const FIntPoint& Resolution) const
{
	const TArrayView<const TSharedPtr<FScreenResolutionEntry>> Resolutions = GetSelectedResolutionList();
	if (Resolutions.IsEmpty())
	{
		return INDEX_NONE;
	}

	int32 ClosestIndex = 0;
	int32 ClosestDiff = TNumericLimits<int32>::Max();
	const int32 TargetSize = Resolution.SizeSquared();
	for (int32 Index = 0; Index < Resolutions.Num(); ++Index)
	{
		if (!Resolutions[Index].IsValid())
		{
			continue;
		}

		const int32 Diff = FMath::Abs(TargetSize - Resolutions[Index]->GetResolution().SizeSquared());
		if (Diff < ClosestDiff)
		{
			ClosestDiff = Diff;
			ClosestIndex = Index;
		}
	}
	return ClosestIndex;
}

void UGSS_GameSettingValueDiscrete_Resolution::GetStandardWindowResolutions(const FIntPoint& MinResolution, const FIntPoint& MaxResolution, float MinAspectRatio, TArray<FIntPoint>& OutResolutions)
{
	static TArray<FIntPoint> StandardResolutions;
	if (StandardResolutions.IsEmpty())
	{
		StandardResolutions = {
			FIntPoint(1024, 768),
			FIntPoint(1366, 768),
			FIntPoint(1360, 768),
			FIntPoint(1280, 800),
			FIntPoint(1152, 864),
			FIntPoint(1440, 900),
			FIntPoint(1280, 1024),
			FIntPoint(1400, 1050),
			FIntPoint(1680, 1050),
			FIntPoint(1600, 1200),
			FIntPoint(1920, 1200),
			FIntPoint(2048, 1152),
			FIntPoint(2048, 1536),
			FIntPoint(2560, 1600),
			FIntPoint(2560, 2048),
			FIntPoint(3200, 2048),
			FIntPoint(3200, 2400),
			FIntPoint(3840, 2400),
			FIntPoint(4096, 3072),
			FIntPoint(5120, 3200),
			FIntPoint(5120, 4096),
			FIntPoint(6400, 4096),
			FIntPoint(6400, 4800),
			FIntPoint(7680, 4800),
			FIntPoint(640, 360),
			FIntPoint(960, 540),
			FIntPoint(1280, 720),
			FIntPoint(1920, 1080),
			FIntPoint(2560, 1440),
			FIntPoint(3200, 1800),
			FIntPoint(3840, 2160),
			FIntPoint(4096, 2160),
			FIntPoint(7680, 4320),
			FIntPoint(5120, 2160),
			FIntPoint(5120, 2880),
			FIntPoint(15360, 8640),
		};
		StandardResolutions.Sort([](const FIntPoint& A, const FIntPoint& B) { return (A.X * A.Y) < (B.X * B.Y); });
	}

	for (const FIntPoint& Resolution : StandardResolutions)
	{
		if (Resolution.X >= MinResolution.X && Resolution.Y >= MinResolution.Y && Resolution.X <= MaxResolution.X && Resolution.Y <= MaxResolution.Y)
		{
			const float AspectRatio = Resolution.Y > 0 ? Resolution.X / static_cast<float>(Resolution.Y) : 0.0f;
			if (AspectRatio > MinAspectRatio || FMath::IsNearlyEqual(AspectRatio, MinAspectRatio))
			{
				OutResolutions.Add(Resolution);
			}
		}
	}
}

FText UGSS_GameSettingValueDiscrete_Resolution::FScreenResolutionEntry::GetDisplayText() const
{
	if (!OverrideText.IsEmpty())
	{
		return OverrideText;
	}

	FText Aspect = FText::GetEmpty();
	const float AspectRatio = Height > 0 ? static_cast<float>(Width) / static_cast<float>(Height) : 0.0f;
	if (FMath::Abs(AspectRatio - (4.0f / 3.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-4:3", "4:3");
	}
	else if (FMath::Abs(AspectRatio - (16.0f / 9.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-16:9", "16:9");
	}
	else if (FMath::Abs(AspectRatio - (16.0f / 10.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-16:10", "16:10");
	}
	else if (FMath::Abs(AspectRatio - (3.0f / 4.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-3:4", "3:4");
	}
	else if (FMath::Abs(AspectRatio - (9.0f / 16.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-9:16", "9:16");
	}
	else if (FMath::Abs(AspectRatio - (10.0f / 16.0f)) < KINDA_SMALL_NUMBER)
	{
		Aspect = LOCTEXT("AspectRatio-10:16", "10:16");
	}

	FNumberFormattingOptions Options;
	Options.UseGrouping = false;

	FFormatNamedArguments Args;
	Args.Add(TEXT("X"), FText::AsNumber(Width, &Options));
	Args.Add(TEXT("Y"), FText::AsNumber(Height, &Options));
	Args.Add(TEXT("AspectRatio"), Aspect);
	return FText::Format(LOCTEXT("ResolutionFormat", "{X} x {Y}"), Args);
}

#undef LOCTEXT_NAMESPACE
