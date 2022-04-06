#pragma once
#include "PlayerState.h"

class MoveState : public PlayerState
{
public:
	MoveState() {}

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);
};

