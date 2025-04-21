using UnrealBuildTool;

public class EchoesOfCreation : ModuleRules
{
    public EchoesOfCreation(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "Niagara",
            "EnhancedInput"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate",
            "SlateCore",
            "UMG"
        });
    }
} 