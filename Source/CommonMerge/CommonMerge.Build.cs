// By hzFishy - 2025 - Do whatever you want with it.

using UnrealBuildTool;

public class CommonMerge : ModuleRules
{
    public CommonMerge(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CommonInventorySystem", "CommonCraftingSystem"
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