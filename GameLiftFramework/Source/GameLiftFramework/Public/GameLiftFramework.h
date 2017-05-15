// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameLiftServerSDK.h"
#include "AwsCoreModule.h"
#include "CognitoIdentityModule.h"
#include "GameLiftModule.h"
#define LOG(x, ... ) UE_LOG(GameLiftFrameworkLog, Log, TEXT("%s - "##x), TEXT(__FUNCTION__), __VA_ARGS__)
#define LOG_WARNING(x, ...) UE_LOG(GameLiftFrameworkLog, Warning,  TEXT("%s - "##x), TEXT(__FUNCTION__), __VA_ARGS__)
#define LOG_ERROR(x, ...) UE_LOG(GameLiftFrameworkLog, Error,  TEXT("%s - "##x), TEXT(__FUNCTION__), __VA_ARGS__)

DECLARE_LOG_CATEGORY_EXTERN(GameLiftFrameworkLog, Log, All);

class GameLiftFrameworkModuleImpl : public IModuleInterface
{
public:
	void StartupModule();
	void ShutdownModule();

private:
};