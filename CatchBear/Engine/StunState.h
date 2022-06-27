#pragma once
#include "PlayerState.h"

class Player;
class StunState : public PlayerState
{
public:
	StunState() {}

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE& ePlayer);
	virtual PlayerState* Update(GameObject& player, STATE& ePlayer);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);


private:
	float _fTime = 0.f;
	STATE	_ePreState;
};
