// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AIPlansEd : ModuleRules
{
	public AIPlansEd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"AIModule",
				"CoreUObject",
				"InputCore",
				"Slate",
				"Engine",
				"Slate",
				"SlateCore",
				"Settings",
				"RenderCore",

				"AIPlans",

				"AssetTools",
				"UnrealEd",
				"KismetWidgets",
				"KismetCompiler",
				"BlueprintGraph",
				"GraphEditor",
				"Kismet",
				"PropertyEditor",
				"EditorStyle",
				"Sequencer",
				"DetailCustomizations",
				"Settings"
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			    "CoreUObject",
			    "InputCore",
			    "Slate",
			    "Engine",
			    "SlateCore",
			    "Settings",
			    "RenderCore",

				"AIPlans",

				"AssetTools",
				"UnrealEd",
				"KismetWidgets",
				"KismetCompiler",
				"BlueprintGraph",
				"GraphEditor",
				"EditorStyle",
				"Sequencer",
				"DetailCustomizations",
				"Settings"
				// ... add private dependencies that you statically link with here ...	
			}
		);		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
