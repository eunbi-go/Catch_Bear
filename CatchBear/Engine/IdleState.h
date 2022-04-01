#pragma once
#include "PlayerState.h"

class IdleState : public PlayerState
{
public:
	IdleState() {}

public:
	virtual void KeyCheck(shared_ptr<Player> player);
	virtual void Update(shared_ptr<Player> player);
};

