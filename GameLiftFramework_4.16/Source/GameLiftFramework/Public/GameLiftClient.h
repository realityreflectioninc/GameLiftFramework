// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "GameLiftClient.generated.h"

/**
 * 
 */
UCLASS()
class GAMELIFTFRAMEWORK_API UGameLiftClient : public UObject
{
	GENERATED_BODY()
	
public:
	void SetUpAwsClient(const char* Region = Aws::Region::AP_NORTHEAST_1, Aws::Http::Scheme Scheme = Aws::Http::Scheme::HTTPS);

	UFUNCTION(BluePrintCallable, Category = "SearchGame")
	void SearchGameSessions();

	virtual void OnSearch();

	void CreatePlayerSession(const Aws::GameLift::Model::GameSession& Session);
	virtual void OnCreatePlayer(const Aws::GameLift::Model::PlayerSession& Session);

	UFUNCTION(BluePrintCallable, Category = "CreateGame")
	void CreateGameSession(const FString& RoomName, const FString& Password, int MaxCount);

	virtual void OnCreateGame(const Aws::GameLift::Model::GameSession& Session);

	std::shared_ptr<Aws::GameLift::GameLiftClient> GetAwsClient() const
	{
		return GLClient;
	}

	UFUNCTION(BluePrintCallable, Category = "GameSession")
	int GetGameSessionCount() const
	{
		return GameSessions.Num();
	}

	const Aws::GameLift::Model::GameSession& GetGameSession(int Index) const
	{
		return GameSessions[Index];
	}

private:
	virtual Aws::Client::ClientConfiguration GetClientConfig(const char* Region = Aws::Region::AP_NORTHEAST_1, Aws::Http::Scheme Scheme = Aws::Http::Scheme::HTTPS) const;

	std::shared_ptr<Aws::GameLift::GameLiftClient> GLClient;
	TArray<Aws::GameLift::Model::GameSession> GameSessions;

	UPROPERTY(EditAnywhere)
	FString AliasId;

	UPROPERTY(EditAnywhere)
	bool IsLocalTest;
	
	UPROPERTY(EditAnywhere)
	FString TestEndpoint;	

	UPROPERTY(EditAnywhere)
	FString PublicKey;

	UPROPERTY(EditAnywhere)
	FString PrivateKey;
};
