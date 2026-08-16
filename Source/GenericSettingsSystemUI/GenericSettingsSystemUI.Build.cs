// Copyright 2026 https://yuewu.dev/en  All Rights Reserved.

using UnrealBuildTool;

public class GenericSettingsSystemUI : ModuleRules
{
	public GenericSettingsSystemUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"Slate",
				"SlateCore",
				"UMG",
				"CommonUI",
				"GenericSettingsSystem",
				"GenericUISystem"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CommonInput"
			});
	}
}
