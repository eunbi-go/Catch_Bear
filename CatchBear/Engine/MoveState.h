#pragma once
#include "PlayerState.h"

class MoveState : public PlayerState
{
public:
	MoveState() {}

public:

	virtual PlayerState* KeyCheck(GameObject& player);
	virtual PlayerState* Update(GameObject& player);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

private:
	float	_fSpeedDownTime = 0.f;
	float	_fSlowSpeed = 5.f;
};

