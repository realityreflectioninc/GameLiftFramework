// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLiftFramework.h"
#include "GameLiftPlayerController.h"
#include "GameLiftGameInstance.h"
#include "GameLiftServerSDK.h"
#include "GameLiftPlayerState.h"



void AGameLiftPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		auto gameLiftInstance = Cast<UGameLiftGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (gameLiftInstance != nullptr)
		{
			auto playerSessionId = gameLiftInstance->PlayerSessionId;
			ServerAcceptPlayer(playerSessionId);
		}
	}
}

void AGameLiftPlayerController::ServerAcceptPlayer_Implementation(const FString& PlayerSessionId)
{
	auto state = Cast<AGameLiftPlayerState>(PlayerState);

	if (state != nullptr)
	{
		state->PlayerSessionId = PlayerSessionId;
	}
}

bool AGameLiftPlayerController::ServerAcceptPlayer_Validate(const FString& PlayerSessionId)
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	auto outcome = gameLiftSdkModule->AcceptPlayerSession(PlayerSessionId);

	return outcome.IsSuccess();
}