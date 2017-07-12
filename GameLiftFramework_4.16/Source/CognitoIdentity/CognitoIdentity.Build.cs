using UnrealBuildTool;
using System.IO;

public class CognitoIdentity : ModuleRules
{
	public CognitoIdentity(ReadOnlyTargetRules Target) : base(Target)
    {
		PrivateIncludePaths.AddRange(new string[] { "CognitoIdentity/Private" });
		PublicIncludePaths.AddRange(new string[] { "CognitoIdentity/Public" });

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "Projects", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "AwsCore" });

        string BaseDirectory = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory, "..", ".."));
        string SDKDirectory = System.IO.Path.Combine(BaseDirectory, "ThirdParty", "CognitoIdentitySDK", Target.Platform.ToString());

        PublicLibraryPaths.Add(SDKDirectory);
        PublicAdditionalLibraries.Add(System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-cognito-identity.lib"));
        PublicDelayLoadDLLs.Add("aws-cpp-sdk-cognito-identity.dll");
        string SDKLibWindows = System.IO.Path.Combine(SDKDirectory, "aws-cpp-sdk-cognito-identity.dll");
        RuntimeDependencies.Add(new RuntimeDependency(SDKLibWindows));
    }
}