// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUIS_DetailSectionsBuilder.generated.h"

class UGUIS_ListEntryDetailSection;
class UGUIS_ListEntry;

/**
 * 继承此类，以自定义如何为一个Object，搜集出所需的
 */
UCLASS(Abstract, Blueprintable)
class GENERICUISYSTEM_API UGUIS_DetailSectionsBuilder : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable, BlueprintNativeEvent)
	TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> GatherDetailSections(const UObject *Data);
	virtual TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> GatherDetailSections_Implementation(const UObject *Data);
};

USTRUCT()
struct FGUIS_EntryDetailsClassSections
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="GUIS")
	TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> Sections;
};

UCLASS(NotBlueprintable)
class GENERICUISYSTEM_API UGUIS_DetailSectionBuilder_Class : public UGUIS_DetailSectionsBuilder

{
	GENERATED_BODY()

public:
	virtual TArray<TSoftClassPtr<UGUIS_ListEntryDetailSection>> GatherDetailSections_Implementation(const UObject *Data) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="GUIS", meta = (AllowAbstract))
	TMap<TSubclassOf<UObject>, FGUIS_EntryDetailsClassSections> SectionsForClasses;
};
