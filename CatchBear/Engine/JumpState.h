#pragma once
#include "PlayerState.h"

class JumpState : public PlayerState
{
public:
	JumpState() {}

public:
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);
};