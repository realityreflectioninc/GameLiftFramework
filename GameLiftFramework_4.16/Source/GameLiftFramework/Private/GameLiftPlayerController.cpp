// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLiftFramework.h"
#include "GameLiftPlayerController.h"
#include "GameLiftGameInstance.h"
#include "GameLiftServerSDK.h"
#include "GameLiftPlayerState.h"
#include "GameLiftGameModeBase.h"



void AGameLiftPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGameLiftPlayerController::ServerAcceptPlayer_Implementation(const FString& PlayerSessionId)
{
	auto state = Cast<AGameLiftPlayerState>(PlayerState);

	if (state != nullptr)
	{
		state->PlayerSessionId = PlayerSessionId;
	}

	auto mode = Cast<AGameLiftGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (mode->IsGameLiftOn())
	{
		FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

		auto outcome = gameLiftSdkModule->AcceptPlayerSession(PlayerSessionId);

		LOG("is success ? %d", outcome.IsSuccess());

	}
}

bool AGameLiftPlayerController::ServerAcceptPlayer_Validate(const FString& PlayerSessionId)
{
	return true;
}

void AGameLiftPlayerController::TryServerAccept_Implementation()
{
#if !WITH_GAMELIFT
	auto gameLiftInstance = Cast<UGameLiftGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gameLiftInstance != nullptr)
	{
		auto playerSessionId = gameLiftInstance->PlayerSessionId;
		ServerAcceptPlayer(playerSessionId);
	}
#endif
}