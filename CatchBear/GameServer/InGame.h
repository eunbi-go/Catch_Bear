#pragma once
class InGame
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);
	void ExceptBroadcast(uint64 exceptPlayerID, SendBufferRef sendBuffer);
	
public:
	bool IsAlreadyEnterPlayer(uint64 player);
	uint64 GetEnterPlayerNum();

	// 인게임에서 모든 플레이어가 접속했는지 검사
	void SetPlayers(PlayerRef player, uint64 playernum);
	void SetPlayerReady(uint64 playerId);
	bool isAllPlayerReady();

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;
	// playerid, player의 ready여부
	map<uint64, bool> _playersReady;
};

extern InGame GInGame;