#pragma once

class Player
{
public:

	uint64					playerId = 0;
	string					nickname;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	GameSessionRef			ownerSession; // Cycle
};

