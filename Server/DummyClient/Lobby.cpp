#include "pch.h"
#include "Lobby.h"
#include "Player.h"
#include "ServerSession.h"

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
	
}
