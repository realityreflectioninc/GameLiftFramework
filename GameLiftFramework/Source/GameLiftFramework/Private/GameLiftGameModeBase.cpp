// Fill out your copyright notice in the Description page of Project Settings.
#include "GameLiftFramework.h"
#include "GameLiftGameModeBase.h"
#include "GameLiftPlayerState.h"
#include "GameLiftPlayerController.h"
#include "UnrealNetwork.h"



void AGameLiftGameModeBase::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	if (!FParse::Param(FCommandLine::Get(), TEXT("local")))
	{
		//Getting the module first.
		FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

		//InitSDK establishes a local connection with GameLift's agent to enable further communication.
		gameLiftSdkModule->InitSDK();

		//When a game session is created, GameLift sends an activation request to the game server and passes along the game session object containing game properties and other settings.
		//Here is where a game server should take action based on the game session object.
		//Once the game server is ready to receive incoming player connections, it should invoke GameLiftServerAPI.ActivateGameSession()
		auto onGameSession = [this](Aws::GameLift::Server::Model::GameSession gameSession)
		{
			OnGameSession(gameSession);
		};

		FProcessParameters* params = new FProcessParameters();
		params->OnStartGameSession.BindLambda(onGameSession);

		//OnProcessTerminate callback. GameLift invokes this callback before shutting down an instance hosting this game server.
		//It gives this game server a chance to save its state, communicate with services, etc., before being shut down.
		//In this case, we simply tell GameLift we are indeed going to shutdown.
		params->OnTerminate.BindLambda([this]() { OnTerminate(); });

		//This is the HealthCheck callback.
		//GameLift invokes this callback every 60 seconds or so.
		//Here, a game server might want to check the health of dependencies and such.
		//Simply return true if healthy, false otherwise.
		//The game server has 60 seconds to respond with its health status. GameLift defaults to 'false' if the game server doesn't respond in time.
		//In this case, we're always healthy!
		params->OnHealthCheck.BindLambda([this]() {return OnHealthCheck(); });

		//This game server tells GameLift that it listens on port 7777 for incoming player connections.
		params->port = 7777;

		//Here, the game server tells GameLift what set of files to upload when the game session ends.
		//GameLift uploads everything specified here for the developers to fetch later.
		TArray<FString> logfiles;
		logfiles.Add(FPaths::Combine(FPaths::GameSavedDir(), TEXT("/Logs/StrikersVR.log")));
		logfiles.Add(FPaths::Combine(FPaths::GameSavedDir(), TEXT("/Logs")));
		logfiles.Add(FPaths::Combine(FPaths::GameSavedDir(), TEXT("/Crashes")));
		params->logParameters = logfiles;

		//Calling ProcessReady tells GameLift this game server is ready to receive incoming game sessions!
		auto outcome = gameLiftSdkModule->ProcessReady(*params);

		bIsGameLiftOn = outcome.IsSuccess();
	}
	else
	{
		bIsGameLiftOn = false;
	}
#endif
	
}

void AGameLiftGameModeBase::OnTerminate()
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	gameLiftSdkModule->ProcessEnding();
}

bool AGameLiftGameModeBase::OnHealthCheck()
{
	return true;
}

void AGameLiftGameModeBase::OnGameSession(Aws::GameLift::Server::Model::GameSession gameSession)
{
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	gameLiftSdkModule->ActivateGameSession();
}

void AGameLiftGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	Cast<AGameLiftPlayerController>(NewPlayer)->TryServerAccept();
}

void AGameLiftGameModeBase::Logout(AController* Existing)
{
	Super::Logout(Existing);

#if WITH_GAMELIFT
	if (IsGameLiftOn())
	{
		FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

		gameLiftSdkModule->RemovePlayerSession(Cast<AGameLiftPlayerState>(Existing->PlayerState)->PlayerSessionId);
	}
#endif
}

void AGameLiftGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameLiftGameModeBase, bIsGameLiftOn);
}