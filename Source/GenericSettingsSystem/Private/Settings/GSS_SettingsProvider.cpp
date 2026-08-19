// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_SettingsProvider.h"

#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_SettingsDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_SettingsProvider)

void UGSS_SettingsBuilder::Initialize(UGSS_SettingsSubsystem* InSubsystem, UGSS_GameSettingRegistry* InRegistry)
{
	Subsystem = InSubsystem;
	Registry = InRegistry;
}

TArray<UGSS_GameSetting*> UGSS_SettingsBuilder::GetCreatedRootSettings() const
{
	TArray<UGSS_GameSetting*> Roots;
	for (UGSS_GameSetting* Setting : CreatedSettings)
	{
		if (Setting && !CreatedSettings.Contains(Setting->GetSettingParent()))
		{
			Roots.Add(Setting);
		}
	}
	return Roots;
}

void UGSS_SettingsBuilder::InitializeDefinition(UGSS_SettingDefinition* Definition, FGameplayTag Id, FText Name, FText Description, const FGSS_SettingValueAccessor& Accessor)
{
	Definition->SettingId = Id;
	Definition->DisplayName = MoveTemp(Name);
	Definition->Description = MoveTemp(Description);
	Definition->Accessor = Accessor;
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddPage(FGameplayTag Id, FText Name, FText Description, FText NavigationText, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_CollectionSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	D->bIsPage = true;
	D->NavigationText = MoveTemp(NavigationText);
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddCollection(FGameplayTag Id, FText Name, FText Description, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_CollectionSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddBool(FGameplayTag Id, FText Name, FText Description, bool DefaultValue, const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_BoolSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = DefaultValue;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddDiscrete(FGameplayTag Id, FText Name, FText Description, FString DefaultValue, const TArray<FGSS_DiscreteOptionDefinition>& Options,
	                                                    const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_DiscreteSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = MoveTemp(DefaultValue);
	D->Options = Options;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddScalar(FGameplayTag Id, FText Name, FText Description, double DefaultValue, double Min, double Max, double Step, const FGSS_SettingValueAccessor& Accessor,
                                                  UGSS_GameSetting* Parent, EGSS_SettingScalarDisplayFormat DisplayFormat)
{
	auto* D = NewObject<UGSS_ScalarSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = DefaultValue;
	D->MinimumValue = Min;
	D->MaximumValue = Max;
	D->Step = Step;
	D->DisplayFormat = DisplayFormat;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddAction(FGameplayTag Id, FText Name, FText Description, FText ActionText, FGameplayTag ActionId, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_ActionSettingDefinition>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	D->ActionText = MoveTemp(ActionText);
	D->ActionId = ActionId;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddDefinition(const UGSS_SettingDefinition* Definition, UGSS_GameSetting* Parent)
{
	if (!Definition || !Registry || !Definition->SettingId.IsValid() || Registry->FindSettingById(Definition->SettingId))
	{
		return nullptr;
	}
	if ((Definition->IsA<UGSS_BoolSettingDefinition>() || Definition->IsA<UGSS_DiscreteSettingDefinition>() || Definition->IsA<UGSS_ScalarSettingDefinition>()) && !Definition->Accessor.IsValid())
	{
		return nullptr;
	}

	UGSS_GameSetting* Setting = nullptr;
	if (const UGSS_CollectionSettingDefinition* Collection = Cast<UGSS_CollectionSettingDefinition>(Definition))
	{
		Setting = Collection->bIsPage
			          ? NewObject<UGSS_GameSettingCollectionPage>(Registry)
			          : NewObject<UGSS_GameSettingCollection>(Registry);
		if (UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting))
		{
			Page->SetNavigationText(Collection->NavigationText);
		}
	}
	else if (const UGSS_BoolSettingDefinition* Bool = Cast<UGSS_BoolSettingDefinition>(Definition))
	{
		UGSS_GameSettingValueDiscreteDynamic_Bool* Value = NewObject<UGSS_GameSettingValueDiscreteDynamic_Bool>(Registry);
		Value->SetDefaultValue(Bool->DefaultValue);
		Value->SetAccessor(Bool->Accessor);
		Setting = Value;
	}
	else if (const UGSS_DiscreteSettingDefinition* Discrete = Cast<UGSS_DiscreteSettingDefinition>(Definition))
	{
		UGSS_GameSettingValueDiscreteDynamic* Value = NewObject<UGSS_GameSettingValueDiscreteDynamic>(Registry);
		Value->SetDefaultValueFromString(Discrete->DefaultValue);
		for (const FGSS_DiscreteOptionDefinition& Option : Discrete->Options) Value->AddDynamicOption(Option.Value, Option.DisplayName);
		Value->SetAccessor(Discrete->Accessor);
		Setting = Value;
	}
	else if (const UGSS_ScalarSettingDefinition* Scalar = Cast<UGSS_ScalarSettingDefinition>(Definition))
	{
		UGSS_GameSettingValueScalarDynamic* Value = NewObject<UGSS_GameSettingValueScalarDynamic>(Registry);
		Value->SetDefaultValue(Scalar->DefaultValue);
		Value->SetSourceRangeAndStep(TRange<double>(Scalar->MinimumValue, Scalar->MaximumValue), Scalar->Step);
		Value->SetAccessor(Scalar->Accessor);
		Value->SetDisplayFormat(UGSS_GameSettingValueScalarDynamic::GetBuiltInDisplayFormat(Scalar->DisplayFormat));
		Setting = Value;
	}
	else if (const UGSS_ActionSettingDefinition* Action = Cast<UGSS_ActionSettingDefinition>(Definition))
	{
		UGSS_GameSettingAction* Value = NewObject<UGSS_GameSettingAction>(Registry);
		Value->SetActionText(Action->ActionText);
		Value->SetNamedAction(Action->ActionId);
		Setting = Value;
	}

	if (!Setting) return nullptr;
	Setting->SetSettingId(Definition->SettingId);
	Setting->SetDevName(Definition->SettingId.GetTagName());
	Setting->SetDisplayName(Definition->DisplayName);
	Setting->SetDescriptionRichText(Definition->Description);
	for (UGSS_SettingEditCondition* ConditionTemplate : Definition->EditConditions)
	{
		if (ConditionTemplate)
		{
			Setting->AddEditCondition(DuplicateObject<UGSS_SettingEditCondition>(ConditionTemplate, Setting));
		}
	}
	Registry->RegisterSetting(Setting, Cast<UGSS_GameSettingCollection>(Parent));
	CreatedSettings.Add(Setting);

	if (const UGSS_CollectionSettingDefinition* Collection = Cast<UGSS_CollectionSettingDefinition>(Definition))
	{
		for (const UGSS_SettingDefinition* Child : Collection->Children)
		{
			if (Child) AddDefinition(Child, Setting);
		}
	}
	return Setting;
}

UGSS_GameSetting* UGSS_SettingsBuilder::AddRuntimeSetting(UGSS_GameSetting* Setting, UGSS_GameSetting* Parent)
{
	if (!Setting || !Registry || !Setting->GetSettingId().IsValid() || Registry->FindSettingById(Setting->GetSettingId()))
	{
		return nullptr;
	}

	Registry->RegisterSetting(Setting, Cast<UGSS_GameSettingCollection>(Parent));
	CreatedSettings.Add(Setting);
	return Setting;
}

void UGSS_SettingsProvider::RegisterSettings_Implementation(UGSS_SettingsBuilder* Builder)
{
}
