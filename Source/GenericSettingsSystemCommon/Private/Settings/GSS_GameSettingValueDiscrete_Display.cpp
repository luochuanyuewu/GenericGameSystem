// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingValueDiscrete_Display.h"

#include "GenericPlatform/GenericApplication.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingValueDiscrete_Display)

#define LOCTEXT_NAMESPACE "GSS_GameSettingValueDiscrete_Display"

void UGSS_GameSettingValueDiscrete_Display::OnInitialized()
{
	RefreshDisplayOptions();
	Super::OnInitialized();
}

void UGSS_GameSettingValueDiscrete_Display::OnDependencyChanged()
{
	RefreshDisplayOptions();
	Super::OnDependencyChanged();
}

void UGSS_GameSettingValueDiscrete_Display::RefreshDisplayOptions()
{
	const TArray<FString> ExistingOptions = GetDynamicOptions();
	for (const FString& Option : ExistingOptions)
	{
		RemoveDynamicOption(Option);
	}

	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	for (const FMonitorInfo& Monitor : DisplayMetrics.MonitorInfo)
	{
		const FText DisplayName = !Monitor.Name.IsEmpty()
			? FText::FromString(Monitor.Name)
			: LOCTEXT("UnknownDisplay", "[Unknown Display]");
		if (!Monitor.ID.IsEmpty())
		{
			AddDynamicOption(Monitor.ID, DisplayName);
		}
	}
}

#undef LOCTEXT_NAMESPACE
