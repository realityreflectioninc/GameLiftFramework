// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "GameLiftPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAMELIFTFRAMEWORK_API AGameLiftPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	FString PlayerSessionId;
private:

};
