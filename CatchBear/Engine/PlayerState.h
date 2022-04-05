#pragma once
#include "Player.h"
class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	virtual PlayerState* KeyCheck(Player& player, STATE ePlayer) = 0;
	virtual PlayerState* Update(Player& player, STATE ePlayer) = 0;
	virtual void Enter(Player& player) = 0;
	virtual void End(Player& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;
	static class JumpState jump;
	static class AttackState attack;
	static class DashState dash;
	static class DashRestState dashRest;

	STATE curState = STATE::END;
};

