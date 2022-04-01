#pragma once
class Player;

class PlayerState
{
public:
	PlayerState() { }
	virtual ~PlayerState() { }

public:
	// 현재 눌린 키를 받아야 함
	virtual PlayerState* KeyCheck(Player& player) = 0;
	virtual void Update(Player& player) = 0;
	virtual void Enter(Player& player) = 0;

public:
	static class IdleState idle;
	static class MoveState move;
};

