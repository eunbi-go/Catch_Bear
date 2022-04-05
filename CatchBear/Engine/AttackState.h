#pragma once

#include "PlayerState.h"
class Player;


class AttackState : public PlayerState
{
public:
	AttackState() {}

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer);
	virtual PlayerState* Update(Player& player, STATE ePlayer);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};

