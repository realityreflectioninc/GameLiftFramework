// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLiftFramework.h"
#include "GameLiftGameInstance.h"
#include "GameLiftClient.h"



UGameLiftGameInstance::UGameLiftGameInstance() 
	: GameLiftClientClass(UGameLiftClient::StaticClass())
{
	Init();
}

void UGameLiftGameInstance::Init()
{
#if !WITH_GAMELIFT
	GameLiftClient = NewObject<UGameLiftClient>(GameLiftClientClass);
#endif
}