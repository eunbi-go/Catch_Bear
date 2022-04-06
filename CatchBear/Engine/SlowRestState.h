#pragma once
#include "PlayerState.h"

class SlowRestState : public PlayerState
{
public:
	SlowRestState() {}
	SlowRestState(float fTime) : _fTime(fTime) {}

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

public:
	void SetSlowTime(float fTime) { _fTime = fTime; }

private:
	float _fTime;
};
