// Copyright Epic Games, Inc. All Rights Reserved.


using System;
using System.IO;
using UnrealBuildTool;

public class SoundVisualizer : ModuleRules
{
	public SoundVisualizer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

 
		  


       string PrjBuildCsFolder = ModuleDirectory; 

        Console.WriteLine(" RulesCompiler : = " + PrjBuildCsFolder);

		string tmpStr = "$(PluginDir)/totozoe";
        Console.WriteLine("  (PluginDir) : =  " + tmpStr);

		string FmodFolder =Path.GetFullPath(Path.Combine(PrjBuildCsFolder, "../../ThirdpartyLibs/FMOD" ) );

        Console.WriteLine(" FmodFolder : = " + FmodFolder);

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	
		PublicIncludePaths.AddRange(new string[] { Path.Combine(FmodFolder, "inc") });

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.GetFullPath( Path.Combine(FmodFolder, "libs/Win64/fmod_vc.lib") ));


            PublicDelayLoadDLLs.Add("fmod.dll");

           RuntimeDependencies.Add(Path.GetFullPath(Path.Combine(FmodFolder, "libs/Win64/fmod.dll")) );



        }
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
            Console.WriteLine("Fix android libs dependencies.......");
        }
		else
		{
			Console.WriteLine("UnSupperted platform.....");
		}


	
	}

    // copy file function
    private void CopyFile(string source, string dest)
    {
        Console.WriteLine("Copying {0} to {1}........", source, dest);
        if (System.IO.File.Exists(dest))
        {
            System.IO.File.SetAttributes(dest, System.IO.File.GetAttributes(dest) & ~System.IO.FileAttributes.ReadOnly);
        }
        try
        {
            System.IO.File.Copy(source, dest, true);
        }
        catch (System.Exception ex)
        {
            Console.WriteLine("Failed to copy file: {0}", ex.Message);
        }
    }
}
