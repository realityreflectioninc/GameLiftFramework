// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLiftFramework.h"
#include "GameLiftClient.h"
#include "GameLiftGameInstance.h"

Aws::Client::ClientConfiguration UGameLiftClient::GetClientConfig(const char* Region, Aws::Http::Scheme Scheme) const
{
	Aws::Client::ClientConfiguration config;

	config.scheme = Scheme;
	config.connectTimeoutMs = 30000;
	config.requestTimeoutMs = 30000;
	config.region = Region;

	if (IsLocalTest)
	{
		config.endpointOverride = TCHAR_TO_UTF8(*TestEndpoint);
	}

	return config;
}

void UGameLiftClient::SetUpAwsClient(const char* Region, Aws::Http::Scheme Scheme)
{
	auto config = GetClientConfig(Region, Scheme);

	auto credential = Aws::Auth::SimpleAWSCredentialsProvider(
						TCHAR_TO_UTF8(*PublicKey), TCHAR_TO_UTF8(*PrivateKey)).GetAWSCredentials();

	GLClient = Aws::MakeShared<Aws::GameLift::GameLiftClient>("GameLiftClient", credential, config);
}

void UGameLiftClient::CreateGameSession(const FString& RoomName, const FString& Password, int MaxCount)
{
	Aws::GameLift::Model::CreateGameSessionRequest req;
	req.SetFleetId(TCHAR_TO_UTF8(*AliasId));
	req.SetName(TCHAR_TO_UTF8(*RoomName));
	req.SetMaximumPlayerSessionCount(MaxCount);

	auto outcome = GLClient->CreateGameSession(req);

	if (outcome.IsSuccess())
	{
		auto gs = outcome.GetResult().GetGameSession();

		OnCreateGame(gs);
	}
	else
	{
		auto msg = Aws::Utils::StringUtils::ToWString(outcome.GetError().GetMessage().c_str());
		LOG_ERROR("fail : %s", msg.c_str());
	}
}

void UGameLiftClient::OnCreateGame(const Aws::GameLift::Model::GameSession& Session)
{
}

void UGameLiftClient::SearchGameSessions()
{
	Aws::GameLift::Model::SearchGameSessionsRequest req;

	req.SetAliasId(TCHAR_TO_UTF8(*AliasId));
	req.SetFilterExpression("hasAvailablePlayerSessions=true");

	auto outcome = GLClient->SearchGameSessions(req);

	if (outcome.IsSuccess())
	{
		GameSessions.Reset();

		auto& games = outcome.GetResult().GetGameSessions();

		for (auto& g : games)
		{
			if (g.GetCurrentPlayerSessionCount() == 0)
				continue;

			GameSessions.Add(g);
		}

		OnSearch();
	}
	else
	{
		auto msg = Aws::Utils::StringUtils::ToWString(outcome.GetError().GetMessage().c_str());
		LOG_ERROR("fail : %s", msg.c_str());
	}
}

void UGameLiftClient::OnSearch()
{
}

void UGameLiftClient::CreatePlayerSession(const Aws::GameLift::Model::GameSession& Session)
{
	Aws::GameLift::Model::CreatePlayerSessionRequest req;
	req.SetGameSessionId(Session.GetGameSessionId().c_str());
	req.SetPlayerId("TestClient");

	auto outcome = GLClient->CreatePlayerSession(req);

	if (outcome.IsSuccess())
	{
		auto& psess = outcome.GetResult().GetPlayerSession();
		auto instance = Cast<UGameLiftGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		
		if (instance != nullptr)
		{
			instance->PlayerSessionId =
				outcome.GetResult().GetPlayerSession().GetPlayerSessionId().c_str();
		}

		OnCreatePlayer(psess);
	}
	else
	{
		auto msg = Aws::Utils::StringUtils::ToWString(outcome.GetError().GetMessage().c_str());
		LOG_ERROR("fail : %s", msg.c_str());
	}
}


void UGameLiftClient::OnCreatePlayer(const Aws::GameLift::Model::PlayerSession& Session)
{
}