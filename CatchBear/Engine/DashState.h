#pragma once
#include "PlayerState.h"
class Player;

class DashState : public PlayerState
{
public:
	DashState() {}
	DashState(float fTime) : _fDashTime(fTime) { }

public: 
	virtual PlayerState* KeyCheck(GameObject& player, STATE& ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE& ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

private:
	float	_fDashTime = 0.f;

	float	_fDashSpeed;
};

