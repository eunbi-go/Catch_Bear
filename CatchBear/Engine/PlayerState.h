#pragma once
class Player;
class GameObject;

class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	// 현재 눌린 키를 받아야 함
	virtual PlayerState* KeyCheck(GameObject& player) = 0;
	virtual void Update(GameObject& player) = 0;
	virtual void Enter(GameObject& player) = 0;
	virtual void End(GameObject& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;

	STATE curState = STATE::END;
};

