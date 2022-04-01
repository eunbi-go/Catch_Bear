#pragma once
class Player;
class IdleState;

class PlayerState
{
public:
	virtual ~PlayerState() { }

public:
	// 현재 눌린 키를 받아야 함
	virtual void KeyCheck(Player& player);
	virtual void Update(shared_ptr<Player> player) {}

public:
	static IdleState idle;
};

