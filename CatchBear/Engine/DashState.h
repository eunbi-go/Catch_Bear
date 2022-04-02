#pragma once
#include "PlayerState.h"
class Player;

class DashState : public PlayerState
{
public:
	DashState() {}
	DashState(float fTime) : _fDashTime(fTime) { }

public: 
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);

private:
	float	_fDashTime = 0.f;
	bool	_isStop = false;
};

