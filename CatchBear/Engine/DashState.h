#pragma once
#include "PlayerState.h"
class Player;

class DashState : public PlayerState
{
public:
	DashState() {}

public:
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);

private:
	float	fDashTime = 0.f;
};

