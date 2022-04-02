#pragma once
#include "PlayerState.h"

class JumpState : public PlayerState
{
public:
	JumpState() {}

public:
	virtual PlayerState* KeyCheck(GameObject& player);
	virtual void Update(GameObject& player);
	virtual void Enter(GameObject& player);
	virtual void End(GameObject& player);
};