#pragma once
class Player;
class GameObject;

class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	virtual PlayerState* KeyCheck(GameObject& player) = 0;
	virtual PlayerState* Update(GameObject& player) = 0;
	virtual void Enter(GameObject& player) = 0;
	virtual void End(GameObject& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;
	static class JumpState jump;
	static class AttackState attack;
	static class DashState dash;
};

