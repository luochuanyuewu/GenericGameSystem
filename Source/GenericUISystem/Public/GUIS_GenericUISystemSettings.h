// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DeveloperSettings.h"
#include "GUIS_GenericUISystemSettings.generated.h"

class UGUIS_GameModalWidget;
class UGUIS_GameUIPolicy;

/**
 *
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Generic UI System Settings"))
class GENERICUISYSTEM_API UGUIS_GenericUISystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="GUIS|Settings", meta = (DisplayName = "Get Generic UI System Settings"))
	static const UGUIS_GenericUISystemSettings *Get();

	/**
	 * Default UI policy to use, Specific GameUILayout through Policy.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="GUIS|Settings")
	TSoftClassPtr<UGUIS_GameUIPolicy> GameUIPolicyClass;

};
