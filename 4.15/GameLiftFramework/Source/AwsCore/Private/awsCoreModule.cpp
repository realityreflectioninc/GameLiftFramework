#include "awsCoreModulePrivatePCH.h"
#include "awsCoreModule.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "AwsCoreModuleImpl"

void* AwsCoreModuleImpl::AwsCoreLibraryHandle = nullptr;

void AwsCoreModuleImpl::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftFramework")->GetBaseDir();
	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("AwsCoreSDK"));
	const FString LibName = TEXT("aws-cpp-sdk-core");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Win64"));
	if (!LoadDependency(LibDir, LibName, AwsCoreLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load aws-cpp-sdk-core library. Plug-in will not be functional."));
		FreeDependency(AwsCoreLibraryHandle);

		return;
	}

	Aws::InitAPI(options);
}

void AwsCoreModuleImpl::ShutdownModule()
{
	if (AwsCoreLibraryHandle == nullptr)
	{
		return;
	}

	FreeDependency(AwsCoreLibraryHandle);

	Aws::ShutdownAPI(options);
}

bool AwsCoreModuleImpl::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
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

void AwsCoreModuleImpl::FreeDependency(void*& Handle)
{
	if(Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(AwsCoreModuleImpl, AwsCore);