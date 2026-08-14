// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

using UnrealBuildTool;

public class GenericSettingsSystem : ModuleRules
{
	public GenericSettingsSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"PropertyPath",
				"UMG"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"CommonUI"
			}
		);
	}
}
