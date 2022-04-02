#pragma once
class Player;

class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	virtual PlayerState* KeyCheck(Player& player) = 0;
	virtual PlayerState* Update(Player& player) = 0;
	virtual void Enter(Player& player) = 0;
	virtual void End(Player& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;
	static class JumpState jump;
	static class AttackState attack;
	static class DashState dash;
};

