#pragma once
#include "PlayerState.h"
class Player;

class DashState : public PlayerState
{
public:
	DashState() {}

public:
	virtual PlayerState* KeyCheck(GameObject& player);
	virtual void Update(GameObject& player);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

private:
	float	fDashTime = 0.f;
};

