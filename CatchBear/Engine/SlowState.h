#pragma once
#include "PlayerState.h"

class SlowState : public PlayerState
{
public:
	SlowState() {}
	SlowState(float fTime) : _fTime(fTime) { }

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer);
	virtual PlayerState* Update(Player& player, STATE ePlayer);
	virtual void Enter(Player& player);
	virtual void End(Player& player);

private:
	float _fTime = 0.f;
};

