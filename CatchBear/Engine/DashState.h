#pragma once
#include "PlayerState.h"
class Player;

class DashState : public PlayerState
{
public:
	DashState() {}
	DashState(float fTime) : _fDashTime(fTime) { }

public: 
	virtual PlayerState* KeyCheck(GameObject& player);
	virtual PlayerState* Update(GameObject& player);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

private:
	float	_fDashTime = 0.f;
	bool	_isStop = false;
};

