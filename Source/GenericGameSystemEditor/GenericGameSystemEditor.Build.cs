// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

using UnrealBuildTool;

public class GenericGameSystemEditor : ModuleRules
{
	public GenericGameSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core"
			});

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"AssetDefinition",
				"AssetTools",
				"GenericEffectsSystem",
				"GenericUISystem",
				"GenericSettingsSystem",
				"GenericSettingsSystemUI",
				"GenericGameSystem"
			});
	}
}
