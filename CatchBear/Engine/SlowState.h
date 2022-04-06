#pragma once
#include "PlayerState.h"

class SlowState : public PlayerState
{
public:
	SlowState() {}
	SlowState(float fTime) : _fTime(fTime) { }

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE& ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

private:
	float _fTime = 0.f;

	float	_fSlowSpeed = 5.f;
	float	_fOriginalSpeed = 10.f;
};

