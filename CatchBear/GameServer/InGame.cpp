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

void InGame::ExceptBroadcast(uint64 exceptPlayerID, SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : _players)
	{
		if (p.second->playerId == exceptPlayerID)
			continue;
		else
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

void InGame::SetPlayers(PlayerRef player, uint64 playernum)
{
	for (int i = 0; i < playernum; ++i)
	{
		if (_playersReady[i] == true)
			continue;
		_playersReady[player->playerId] = false;

	}
}

void InGame::SetPlayerReady(uint64 playerId)
{
	WRITE_LOCK;
	_playersReady[playerId] = true;
}

bool InGame::isAllPlayerReady()
{
	WRITE_LOCK;
	int isize = _playersReady.size();

	for (int i = 0; i < isize; ++i)
	{
		if (_playersReady[i] == false)
			return false;
	}
	return true;
}
