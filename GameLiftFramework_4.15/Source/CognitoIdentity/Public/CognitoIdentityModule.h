#pragma once

#include "ModuleManager.h"

class CognitoIdentityModuleImpl : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:

	static void* CognitoIdentityLibraryHandle;
	static bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	static void FreeDependency(void*& Handle);
};