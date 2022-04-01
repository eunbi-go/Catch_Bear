#pragma once

#include "PlayerState.h"
class Player;


class AttackState : public PlayerState
{
public:
	AttackState() {}

public:
	virtual PlayerState* KeyCheck(GameObject& player);
	virtual PlayerState* Update(GameObject& player);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);
};

