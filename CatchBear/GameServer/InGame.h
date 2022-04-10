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

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;
};

extern InGame GInGame;