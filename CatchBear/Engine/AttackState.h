#pragma once

#include "PlayerState.h"
class Player;


class AttackState : public PlayerState
{
public:
	AttackState() {}

public:
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};

