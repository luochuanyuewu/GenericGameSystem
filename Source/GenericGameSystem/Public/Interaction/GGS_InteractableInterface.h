// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GGS_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class GENERICGAMESYSTEM_API UGGS_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Optional interface for actor to responds to interaction events.
 * General usage of this interface is to do some visual stuff.
 */
class GENERICGAMESYSTEM_API IGGS_InteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Called when this actor was selected by interaction system.
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="GGS|Interaction")
	void OnInteractionSelected(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="GGS|Interaction")
	void OnInteractionDeselected(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="GGS|Interaction")
	void OnInteractionBegin(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="GGS|Interaction")
	void OnInteractionEnd(AActor* Instigator);
};
