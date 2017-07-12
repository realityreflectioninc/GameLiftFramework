// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "GameLiftGameInstance.generated.h"

class UGameLiftClient;

/**
 * 
 */
UCLASS()
class GAMELIFTFRAMEWORK_API UGameLiftGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameLiftGameInstance();

	FString PlayerSessionId;
	
	UGameLiftClient* GetGameLiftClient() const
	{
#if WITH_GAMELIFT
		return nullptr;
#else
		return GameLiftClient;
#endif
	}

	virtual void Init() override;
	
private:
	UPROPERTY(EditAnywhere)
	UClass* GameLiftClientClass;

#if !WITH_GAMELIFT
	UGameLiftClient* GameLiftClient;
#endif
};
