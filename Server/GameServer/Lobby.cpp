#include "pch.h"
#include "Lobby.h"
#include "Player.h"
#include "GameSession.h"

Lobby GLobby;

void Lobby::Enter(PlayerRef player)
{
	WRITE_LOCK;
	_players[player->playerId] = player;
}

void Lobby::Leave(PlayerRef player)
{
	WRITE_LOCK;
	_players.erase(player->playerId);
}

void Lobby::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : _players)
	{
		p.second->ownerSession->Send(sendBuffer);
	}
}

bool Lobby::isFirstEnterLobby(uint64 playerId)
{
	WRITE_LOCK;
	auto key = _players.find(playerId);
	if (key != _players.end())
		return false;
	else
		return true;
}
