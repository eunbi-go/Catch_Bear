#pragma once
#include "PlayerState.h"
class Player;
class IdleState : public PlayerState
{
public:
	IdleState() {}

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer);
	virtual PlayerState* Update(Player& player, STATE ePlayer);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};

