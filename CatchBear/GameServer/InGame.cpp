#include "pch.h"
#include "InGame.h"
#include "Player.h"
#include "GameSession.h"

InGame GInGame;

void InGame::Enter(PlayerRef player)
{
	WRITE_LOCK;
	_players[player->playerId] = player;
}

void InGame::Leave(PlayerRef player)
{
	WRITE_LOCK;
	_players.erase(player->playerId);
}

void InGame::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : _players)
	{
		p.second->ownerSession->Send(sendBuffer);
	}
}

bool InGame::IsAlreadyEnterPlayer(uint64 playerId)
{
	for (auto& p : _players)
	{
		if (p.second->playerId == playerId)
			return true;
	}
	return false;
}

uint64 InGame::GetEnterPlayerNum()
{
	return _players.size();
}

void InGame::mySend(SendBufferRef sendbuf)
{
	WRITE_LOCK;
	_players[0]->ownerSession->Send(sendbuf);
}
