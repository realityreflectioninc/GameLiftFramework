using UnrealBuildTool;
using System.IO;

public class AwsCore : ModuleRules
{
	public AwsCore(ReadOnlyTargetRules Target) : base(Target)
    {
		PrivateIncludePaths.AddRange(new string[] { "AwsCore/Private" });
		PublicIncludePaths.AddRange(new string[] { "AwsCore/Public" });

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "Projects", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string SDKDirectory = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "AwsCoreSDK", Target.Platform.ToString());

        PublicLibraryPaths.Add(SDKDirectory);
        PublicAdditionalLibraries.Add(System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-core.lib"));
        PublicDelayLoadDLLs.Add("aws-cpp-sdk-core.dll");
        string SDKLibWindows = System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-core.dll");
        RuntimeDependencies.Add(new RuntimeDependency(SDKLibWindows));
    }
}