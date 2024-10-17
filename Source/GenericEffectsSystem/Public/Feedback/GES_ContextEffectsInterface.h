// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "Engine/HitResult.h"
#include "GameplayTagContainer.h"
#include "GES_ContextEffectsStructLibrary.h"
#include "UObject/Interface.h"

#include "GES_ContextEffectsInterface.generated.h"

class UAnimSequenceBase;
class UObject;
class USceneComponent;
struct FFrame;


/**
 * Any Object implement this interface can responses to context effects.
 */
UINTERFACE(Blueprintable)
class GENERICEFFECTSSYSTEM_API UGES_ContextEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class GENERICEFFECTSSYSTEM_API IGES_ContextEffectsInterface : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GES|ContextEffect")
	void PlayContextEffectsWithInput(FGES_SpawnContextEffectsInput Input);
	virtual void PlayContextEffectsWithInput_Implementation(FGES_SpawnContextEffectsInput Input) = 0;
};
