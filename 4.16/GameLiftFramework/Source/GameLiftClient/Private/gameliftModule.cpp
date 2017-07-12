#include "gameliftModulePrivatePCH.h"
#include "gameliftModule.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "GameLiftClientModuleImpl"

void* GameLiftClientModuleImpl::GameLiftClientLibraryHandle = nullptr;

void GameLiftClientModuleImpl::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("GameLiftFramework")->GetBaseDir();
	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("ThirdParty"), TEXT("GameLiftClientSDK"));
	const FString LibName = TEXT("aws-cpp-sdk-gamelift");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Win64"));
	if (!LoadDependency(LibDir, LibName, GameLiftClientLibraryHandle))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load aws-cpp-sdk-gamelift library. Plug-in will not be functional."));
		FreeDependency(GameLiftClientLibraryHandle);

		return;
	}
}

void GameLiftClientModuleImpl::ShutdownModule()
{
	if (GameLiftClientLibraryHandle == nullptr)
	{
		return;
	}

	FreeDependency(GameLiftClientLibraryHandle);
}

bool GameLiftClientModuleImpl::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
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

void GameLiftClientModuleImpl::FreeDependency(void*& Handle)
{
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(GameLiftClientModuleImpl, GameLiftClient);