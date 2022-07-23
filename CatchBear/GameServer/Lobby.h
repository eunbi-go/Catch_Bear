#pragma once

class Lobby
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

	bool isFirstEnterLobby(uint64 playerId);
	void SetPlayerReady(uint64 playerId, bool _isReady);
	bool GetPlayerReady(uint64 playerId);
	void SetPlayerType(uint64 playerId, uint64 _type);
	uint64 GetPlayerType(uint64 playerId);

	bool isAllPlayerReady();
	bool isPlayerEnterLobby(uint64 playerId);

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;
	// playerid, player¿« readyø©∫Œ
	map<uint64, bool> _playersReady;
	map<uint64, uint64> _playerType;
};

extern Lobby GLobby;