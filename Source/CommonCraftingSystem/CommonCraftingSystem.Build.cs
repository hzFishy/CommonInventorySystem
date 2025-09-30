// By hzFishy - 2025 - Do whatever you want with it.

using UnrealBuildTool;

public class CommonCraftingSystem : ModuleRules
{
    public CommonCraftingSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "CommonTypes",
                "GameplayTags"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject", "Engine",
                "Slate", "SlateCore"
            }
        );
    }
}