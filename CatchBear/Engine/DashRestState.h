#pragma once
#include "PlayerState.h"

class DashRestState : public PlayerState
{
public:
	DashRestState() {}
	DashRestState(float fTime) : _fDashTime(fTime) {}

public:
	virtual PlayerState* KeyCheck(Player& player);
	virtual PlayerState* Update(Player& player);
	virtual void Enter(Player& player);
	virtual void End(Player& player);

public:
	void SetDashTime(float fTime) { _fDashTime = fTime; }

private:
	float _fDashTime;
};
