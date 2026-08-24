// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

#include "Settings/GSS_GameSettingsProvider.h"

#include "Settings/GSS_GameSetting.h"
#include "Settings/GSS_GameSettingAction.h"
#include "Settings/GSS_GameSettingCollection.h"
#include "Settings/GSS_GameSettingRegistry.h"
#include "Settings/GSS_GameSettingValueDiscreteDynamic.h"
#include "Settings/GSS_GameSettingValueScalarDynamic.h"
#include "Settings/GSS_GameSettingsDefinitions.h"
#include "GenericSettingsSystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GSS_GameSettingsProvider)

void UGSS_GameSettingsBuilder::Initialize(UGSS_GameSettingsSubsystem* InSubsystem, UGSS_GameSettingRegistry* InRegistry)
{
	Subsystem = InSubsystem;
	Registry = InRegistry;
}

TArray<UGSS_GameSetting*> UGSS_GameSettingsBuilder::GetCreatedRootSettings() const
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

void UGSS_GameSettingsBuilder::InitializeDefinition(UGSS_GameSettingDefinition* Definition, FGameplayTag Id, FText Name, FText Description, const FGSS_SettingValueAccessor& Accessor)
{
	Definition->SettingId = Id;
	Definition->DisplayName = MoveTemp(Name);
	Definition->Description = MoveTemp(Description);
	Definition->Accessor = Accessor;
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddPage(FGameplayTag Id, FText Name, FText Description, FText NavigationText, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Collection>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	D->bIsPage = true;
	D->NavigationText = MoveTemp(NavigationText);
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddCollection(FGameplayTag Id, FText Name, FText Description, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Collection>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddBool(FGameplayTag Id, FText Name, FText Description, bool DefaultValue, const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Bool>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = DefaultValue;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddDiscrete(FGameplayTag Id, FText Name, FText Description, FString DefaultValue, const TArray<FGSS_DiscreteOptionDefinition>& Options,
	                                                    const FGSS_SettingValueAccessor& Accessor, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Discrete>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = MoveTemp(DefaultValue);
	D->Options = Options;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddScalar(FGameplayTag Id, FText Name, FText Description, double DefaultValue, double Min, double Max, double Step, const FGSS_SettingValueAccessor& Accessor,
                                                  UGSS_GameSetting* Parent, EGSS_SettingScalarDisplayFormat DisplayFormat)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Scalar>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), Accessor);
	D->DefaultValue = DefaultValue;
	D->MinimumValue = Min;
	D->MaximumValue = Max;
	D->Step = Step;
	D->DisplayFormat = DisplayFormat;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddAction(FGameplayTag Id, FText Name, FText Description, FText ActionText, FGameplayTag ActionId, UGSS_GameSetting* Parent)
{
	auto* D = NewObject<UGSS_GameSettingDefinition_Action>(this);
	InitializeDefinition(D, Id, MoveTemp(Name), MoveTemp(Description), FGSS_SettingValueAccessor());
	D->ActionText = MoveTemp(ActionText);
	D->ActionId = ActionId;
	return AddDefinition(D, Parent);
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddDefinition(const UGSS_GameSettingDefinition* Definition, UGSS_GameSetting* Parent)
{
	if (!Definition || !Registry || !Definition->SettingId.IsValid() || Registry->FindSettingById(Definition->SettingId))
	{
		return nullptr;
	}
	if (Definition->IsA<UGSS_GameSettingDefinition_Bool>() || Definition->IsA<UGSS_GameSettingDefinition_Discrete>() || Definition->IsA<UGSS_GameSettingDefinition_Scalar>())
	{
		FString AccessorError;
		if (!Definition->Accessor.Validate(AccessorError))
		{
			UE_LOG(LogGSS, Error, TEXT("Rejected setting %s: %s"), *Definition->SettingId.ToString(), *AccessorError);
			return nullptr;
		}
	}

	UGSS_GameSetting* Setting = nullptr;
	if (const UGSS_GameSettingDefinition_Collection* Collection = Cast<UGSS_GameSettingDefinition_Collection>(Definition))
	{
		Setting = Collection->bIsPage
			          ? NewObject<UGSS_GameSettingCollectionPage>(Registry)
			          : NewObject<UGSS_GameSettingCollection>(Registry);
		if (UGSS_GameSettingCollectionPage* Page = Cast<UGSS_GameSettingCollectionPage>(Setting))
		{
			Page->SetNavigationText(Collection->NavigationText);
		}
	}
	else if (const UGSS_GameSettingDefinition_Bool* Bool = Cast<UGSS_GameSettingDefinition_Bool>(Definition))
	{
		UGSS_GameSettingValueDiscreteDynamic_Bool* Value = NewObject<UGSS_GameSettingValueDiscreteDynamic_Bool>(Registry);
		Value->SetDefaultValue(Bool->DefaultValue);
		Value->SetAccessor(Bool->Accessor);
		Setting = Value;
	}
	else if (const UGSS_GameSettingDefinition_Discrete* Discrete = Cast<UGSS_GameSettingDefinition_Discrete>(Definition))
	{
		UGSS_GameSettingValueDiscreteDynamic* Value = NewObject<UGSS_GameSettingValueDiscreteDynamic>(Registry);
		Value->SetDefaultValueFromString(Discrete->DefaultValue);
		for (const FGSS_DiscreteOptionDefinition& Option : Discrete->Options) Value->AddDynamicOption(Option.Value, Option.DisplayName);
		Value->SetAccessor(Discrete->Accessor);
		Setting = Value;
	}
	else if (const UGSS_GameSettingDefinition_Scalar* Scalar = Cast<UGSS_GameSettingDefinition_Scalar>(Definition))
	{
		UGSS_GameSettingValueScalarDynamic* Value = NewObject<UGSS_GameSettingValueScalarDynamic>(Registry);
		Value->SetDefaultValue(Scalar->DefaultValue);
		Value->SetSourceRangeAndStep(TRange<double>(Scalar->MinimumValue, Scalar->MaximumValue), Scalar->Step);
		Value->SetAccessor(Scalar->Accessor);
		Value->SetDisplayFormat(UGSS_GameSettingValueScalarDynamic::GetBuiltInDisplayFormat(Scalar->DisplayFormat));
		Setting = Value;
	}
	else if (const UGSS_GameSettingDefinition_Action* Action = Cast<UGSS_GameSettingDefinition_Action>(Definition))
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
	for (UGSS_GameSettingEditCondition* ConditionTemplate : Definition->EditConditions)
	{
		if (ConditionTemplate)
		{
			Setting->AddEditCondition(DuplicateObject<UGSS_GameSettingEditCondition>(ConditionTemplate, Setting));
		}
	}
	Setting->AddEditDependencies(Definition->EditDependencies);
	Registry->RegisterSetting(Setting, Cast<UGSS_GameSettingCollection>(Parent));
	CreatedSettings.Add(Setting);

	if (const UGSS_GameSettingDefinition_Collection* Collection = Cast<UGSS_GameSettingDefinition_Collection>(Definition))
	{
		for (const UGSS_GameSettingDefinition* Child : Collection->Children)
		{
			if (Child) AddDefinition(Child, Setting);
		}
	}
	return Setting;
}

UGSS_GameSetting* UGSS_GameSettingsBuilder::AddRuntimeSetting(UGSS_GameSetting* Setting, UGSS_GameSetting* Parent)
{
	if (!Setting || !Registry || !Setting->GetSettingId().IsValid() || Registry->FindSettingById(Setting->GetSettingId()))
	{
		return nullptr;
	}

	Registry->RegisterSetting(Setting, Cast<UGSS_GameSettingCollection>(Parent));
	CreatedSettings.Add(Setting);
	return Setting;
}

void UGSS_GameSettingsProvider::RegisterSettings_Implementation(UGSS_GameSettingsBuilder* Builder)
{
}
