// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GGS_SocketRelationshipMapping.generated.h"

USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_SocketAdjustment
{
	GENERATED_BODY()

	/**
	 * Name of skeletons.
	 */
	UPROPERTY(EditAnywhere, Category="GGS")
	TArray<FString> ForSkeletons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GGS")
	FName SocketName{NAME_None};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GGS")
	FTransform RelativeTransform;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category="GGS", meta=(EditCondition=false, EditConditionHides))
	FString EditorFriendlyName;
#endif
};

USTRUCT(BlueprintType)
struct GENERICGAMESYSTEM_API FGGS_SocketRelationship
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GGS")
	TSoftObjectPtr<UStreamableRenderAsset> MeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GGS", meta=(TitleProperty="EditorFriendlyName"))
	TArray<FGGS_SocketAdjustment> Adjustments;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category="GGS", meta=(EditCondition=false, EditConditionHides))
	FString EditorFriendlyName;
#endif
};


/**
 * Defines the socket relationship of meshes to parent meshes when attached to different socket.
 */
UCLASS(BlueprintType)
class GENERICGAMESYSTEM_API UGGS_SocketRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure=False, Category="GGS|Utilities")
	bool FindSocketAdjustment(const USkeletalMeshComponent* InParentMeshComponent, const UStreamableRenderAsset* InMeshAsset, FName InSocketName,
	                          FGGS_SocketAdjustment& OutAdjustment) const;

	UFUNCTION(BlueprintCallable, Category="GGS|Utilities")
	static bool FindSocketAdjustmentInMappings(TArray<TSoftObjectPtr<UGGS_SocketRelationshipMapping>> InMappings, const USkeletalMeshComponent* InParentMeshComponent,
	                                           const UStreamableRenderAsset* InMeshAsset, FName InSocketName,
	                                           FGGS_SocketAdjustment& OutAdjustment);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GGS", meta=(TitleProperty="EditorFriendlyName"))
	TArray<FGGS_SocketRelationship> Relationships;


#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
};
