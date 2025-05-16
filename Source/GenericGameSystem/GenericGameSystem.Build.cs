// Copyright 2024 https://yuewu.dev/en  All Rights Reserved.
using UnrealBuildTool;

public class GenericGameSystem : ModuleRules
{
    public GenericGameSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "SmartObjectsModule"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "NetCore",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayTags",
                "UMG",
                "StructUtils",
                "TargetingSystem",
                "GameplayTasks",
                "GameplayAbilities",
                "GameplayBehaviorsModule",
                "SmartObjectsModule",
                "GameplayBehaviorSmartObjectsModule"
            }
        );
    }
}