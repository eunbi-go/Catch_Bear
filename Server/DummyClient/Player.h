#pragma once

class Player
{
public:
	uint64					playerId = 0;
	string					name;
	Protocol::PlayerType	type = Protocol::PLAYER_TYPE_NONE;
	// GameSessionRef			ownerSession; // Cycle
	 
	// Lobby 씬에서의 레디 여부
	bool					bisPlayerReady = false;
};

extern Player GPlayer;