using UnrealBuildTool;
using System.IO;

public class GameLiftClient : ModuleRules
{
	public GameLiftClient(TargetInfo Target)
	{
		PrivateIncludePaths.AddRange(new string[] { "GameLiftClient/Private" });
		PublicIncludePaths.AddRange(new string[] { "GameLiftClient/Public" });

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "AwsCore" });

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string SDKDirectory = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "GameLiftClientSDK", Target.Platform.ToString());

		PublicLibraryPaths.Add(SDKDirectory);
		PublicAdditionalLibraries.Add(System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-gamelift.lib"));
        PublicDelayLoadDLLs.Add("aws-cpp-sdk-gamelift.dll");
        string SDKLibWindows = System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-gamelift.dll");
        RuntimeDependencies.Add(new RuntimeDependency(SDKLibWindows));
	}
}