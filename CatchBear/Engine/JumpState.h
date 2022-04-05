#pragma once
#include "PlayerState.h"

class JumpState : public PlayerState
{
public:
	JumpState() {}

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer);
	virtual PlayerState* Update(Player& player, STATE ePlayer);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};