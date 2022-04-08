#pragma once
#include "PlayerState.h"

class DashRestState : public PlayerState
{
public:
	DashRestState() {}
	DashRestState(float fTime) : _fDashTime(fTime) {}

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE& ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE& ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);

public:
	void SetDashTime(float fTime) { _fDashTime = fTime; }

private:
	float _fDashTime;
};
