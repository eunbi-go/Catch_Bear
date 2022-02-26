#pragma once

class Lobby
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

	bool isFirstEnterLobby(uint64 playerId);
	void SetPlayerReady(uint64 playerId);
	bool isAllPlayerReady();

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;
	// playerid, player¿« readyø©∫Œ
	map<uint64, bool> _playersReady;
};

extern Lobby GLobby;