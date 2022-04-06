#pragma once
#include "Player.h"
class GameObject;
class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	virtual PlayerState* KeyCheck(GameObject& player, STATE ePlayer) = 0;
	virtual PlayerState* Update(GameObject& player, STATE ePlayer) = 0;
	virtual void Enter(GameObject& player) = 0;
	virtual void End(GameObject& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;
	static class JumpState jump;
	static class AttackState attack;
	static class DashState dash;
	static class DashRestState dashRest;
	static class SlowState slow;
	static class SlowRestState slowRest;
	static class StunState stun;

	STATE curState = STATE::END;
};

