// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameLiftServerSDK.h"
#include "AwsCoreModule.h"
#include "CognitoIdentityModule.h"
#include "GameLiftModule.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMELIFTFRAMEWORK_API AGameLiftGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Existing) override;

	bool IsGameLiftOn() const { return bIsGameLiftOn; }

	virtual void OnGameSession(Aws::GameLift::Server::Model::GameSession gameSession);

	virtual void OnTerminate();

	virtual bool OnHealthCheck();
	
private:

	UPROPERTY(Replicated)
	bool bIsGameLiftOn;
};
