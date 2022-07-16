#include "pch.h"
#include "Lobby.h"
#include "Player.h"
#include "GameSession.h"

Lobby GLobby;

void Lobby::Enter(PlayerRef player)
{
	WRITE_LOCK;
	_players[player->playerId] = player;
	_playersReady[player->playerId] = false;
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

void Lobby::SetPlayerReady(uint64 playerId)
{
	WRITE_LOCK;
	_playersReady[playerId] = true;
}

bool Lobby::isAllPlayerReady()
{
	WRITE_LOCK;
	//auto key = _playersReady.find(false);
	//// 모든 플레이어가 준비를 했다면?
	//if (key == _playersReady.end())
	//	return true;
	//else
	//	return false;

	//if (_playersReady.find(false) == _playersReady.end())
	//	return true;
	//else
	//	return false;

	int isize = _playersReady.size();

	for (int i = 0; i < isize; ++i)
	{
		if (_playersReady[i] == false)
			return false;
	}
	return true;
}

bool Lobby::isPlayerEnterLobby(uint64 playerId)
{
	if (_players[playerId] == nullptr)
		return false;
	return true;
}
