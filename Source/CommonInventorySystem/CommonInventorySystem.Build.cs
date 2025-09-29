// By hzFishy - 2025 - Do whatever you want with it.

using UnrealBuildTool;

public class CommonInventorySystem : ModuleRules
{
	public CommonInventorySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange( new string[]
		{
			"Core",
			"DeveloperSettings",
			"GameplayTags",
			"FishyUtils"
		});
			
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject", "Engine",
			"Slate", "SlateCore",
		});
	}
}
