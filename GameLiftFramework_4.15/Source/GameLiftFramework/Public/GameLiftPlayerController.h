// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "GameLiftPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMELIFTFRAMEWORK_API AGameLiftPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerAcceptPlayer(const FString& PlayerSessionId);
	
	UFUNCTION(Client, Reliable)
	void TryServerAccept();
	
};
