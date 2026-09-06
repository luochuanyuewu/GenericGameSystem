// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

using UnrealBuildTool;

public class GenericSettingsSystemCommon : ModuleRules
{
	public GenericSettingsSystemCommon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"GenericSettingsSystem"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"RHI",
				"Slate",
				"SlateCore"
			});
	}
}
