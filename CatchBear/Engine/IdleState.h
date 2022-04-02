#pragma once
#include "PlayerState.h"
class Player;
class IdleState : public PlayerState
{
public:
	IdleState() {}

public:
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};

