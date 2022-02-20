#pragma once

class Lobby
{
public:
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	map<uint64, PlayerRef> _players;
};

extern Lobby GLobby;