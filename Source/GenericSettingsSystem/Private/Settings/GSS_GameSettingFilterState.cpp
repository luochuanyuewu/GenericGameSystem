// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingFilterState.h"
#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingEditCondition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingFilterState)

#define LOCTEXT_NAMESPACE "GSS_GameSetting"

class FSettingFilterExpressionContext : public ITextFilterExpressionContext
{
public:
	explicit FSettingFilterExpressionContext(const UGSS_GameSetting& InSetting) : Setting(InSetting) {}

	virtual bool TestBasicStringExpression(const FTextFilterString& InValue, const ETextFilterTextComparisonMode InTextComparisonMode) const override
	{
		return TextFilterUtils::TestBasicStringExpression(Setting.GetDescriptionPlainText(), InValue, InTextComparisonMode);
	}

	virtual bool TestComplexExpression(const FName& InKey, const FTextFilterString& InValue, const ETextFilterComparisonOperation InComparisonOperation, const ETextFilterTextComparisonMode InTextComparisonMode) const override
	{
		return false;
	}

private:
	/** Setting being filtered. */
	const UGSS_GameSetting& Setting;
};

//--------------------------------------
// FGSS_GameSettingFilterState
//--------------------------------------

FGSS_GameSettingEditableState::FGSS_GameSettingEditableState() = default;

void FGSS_GameSettingEditableState::Hide(const FString& DeveloperReason)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!DeveloperReason.IsEmpty(), TEXT("A developer reason is required when hiding a setting."));
	HiddenReasons.Add(DeveloperReason);
#endif
	bVisible = false;
}

void FGSS_GameSettingEditableState::Disable(const FText& Reason)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!Reason.IsEmpty(), TEXT("A player-facing reason is required when disabling a setting."));
#endif
	bEnabled = false;
	DisabledReasons.Add(Reason);
}

void FGSS_GameSettingEditableState::DisableOption(const FString& OptionValue)
{
#if !UE_BUILD_SHIPPING
	ensureAlwaysMsgf(!DisabledOptions.Contains(OptionValue), TEXT("The setting option is already disabled."));
#endif
	DisabledOptions.Add(OptionValue);
}

void FGSS_GameSettingEditableState::PreventReset()
{
	bResettable = false;
}

void FGSS_GameSettingEditableState::Kill(const FString& DeveloperReason)
{
	Hide(DeveloperReason);
	PreventReset();
}

FGSS_GameSettingFilterState::FGSS_GameSettingFilterState()
	: SearchTextEvaluator(ETextFilterExpressionEvaluatorMode::BasicString)
{
}

void FGSS_GameSettingFilterState::AddSettingToRootList(UGSS_GameSetting* InSetting)
{
	SettingAllowList.Add(InSetting);
	SettingRootList.Add(InSetting);
}

void FGSS_GameSettingFilterState::AddSettingToAllowList(UGSS_GameSetting* InSetting)
{
	SettingAllowList.Add(InSetting);
}

void FGSS_GameSettingFilterState::SetSearchText(const FString& InSearchText)
{
	SearchTextEvaluator.SetFilterText(FText::FromString(InSearchText));
}

bool FGSS_GameSettingFilterState::DoesSettingPassFilter(const UGSS_GameSetting& InSetting) const
{
	const FGSS_GameSettingEditableState& EditableState = InSetting.GetEditState();

	if (!bIncludeHidden && !EditableState.IsVisible())
	{
		return false;
	}

	if (!bIncludeDisabled && !EditableState.IsEnabled())
	{
		return false;
	}

	if (!bIncludeResetable && !EditableState.IsResettable())
	{
		return false;
	}

	// Are we filtering settings?
	if (SettingAllowList.Num() > 0)
	{
		if (!SettingAllowList.Contains(&InSetting))
		{
			bool bAllowed = false;
			const UGSS_GameSetting* NextSetting = &InSetting;
			while (const UGSS_GameSetting* Parent = NextSetting->GetSettingParent())
			{
				if (SettingAllowList.Contains(Parent))
				{
					bAllowed = true;
					break;
				}

				NextSetting = Parent;
			}

			if (!bAllowed)
			{
				return false;
			}
		}
	}

	// TODO more filters...

	// Always search text last, it's generally the most expensive filter.
	if (!SearchTextEvaluator.TestTextFilter(FSettingFilterExpressionContext(InSetting)))
	{
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
