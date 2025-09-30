// By hzFishy - 2025 - Do whatever you want with it.

using UnrealBuildTool;

public class CommonTypes : ModuleRules
{
    public CommonTypes(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "GameplayTags"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject" ,
                "Slate", "SlateCore"
            }
        );
    }
}