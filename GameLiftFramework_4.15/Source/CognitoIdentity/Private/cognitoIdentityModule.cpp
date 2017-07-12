#include "cognitoIdentityModulePrivatePCH.h"
#include "cognitoIdentityModule.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "CognitoIdentityModuleImpl"

void* CognitoIdentityModuleImpl::CognitoIdentityLibraryHandle = nullptr;

void CognitoIdentityModuleImpl::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftFramework")->GetBaseDir();
	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("CognitoIdentitySDK"));
	const FString LibName = TEXT("aws-cpp-sdk-cognito-identity");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Win64"));
	if (!LoadDependency(LibDir, LibName, CognitoIdentityLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load aws-cpp-sdk-cognito-identity library. Plug-in will not be functional."));
		FreeDependency(CognitoIdentityLibraryHandle);

		return;
	}
}

void CognitoIdentityModuleImpl::ShutdownModule()
{
	if (CognitoIdentityLibraryHandle == nullptr)
	{
		return;
	}

	FreeDependency(CognitoIdentityLibraryHandle);
}

bool CognitoIdentityModuleImpl::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if (Handle == nullptr)
	{
		return false;
	}

	return true;
}

void CognitoIdentityModuleImpl::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(CognitoIdentityModuleImpl, CognitoIdentity);