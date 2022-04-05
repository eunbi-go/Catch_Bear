#pragma once
#include "PlayerState.h"

class SlowRestState : public PlayerState
{
public:
	SlowRestState() {}
	SlowRestState(float fTime) : _fTime(fTime) {}

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer);
	virtual PlayerState* Update(Player& player, STATE ePlayer);
	virtual void Enter(Player& player);
	virtual void End(Player& player);

public:
	void SetSlowTime(float fTime) { _fTime = fTime; }

private:
	float _fTime;
};
