// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.


using UnrealBuildTool;

public class GenericUISystem : ModuleRules
{
    public GenericUISystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CommonUI",
                "Slate",
                "UMG"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "SlateCore",
                "ApplicationCore",
                "EnhancedInput",
                "PropertyPath",
                "GameplayTags",
                "InputCore",
                "CommonInput",
                "DeveloperSettings",
                "ModularGameplay"
            }
        );
    }
}