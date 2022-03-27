#pragma once
#include "MonoBehaviour.h"

class Player : public MonoBehaviour
{
	enum STATE
	{
		IDLE, WALK, DASH, JUMP, ATTACK, END
	};

public:
	Player();
	virtual ~Player();

	virtual void LateUpdate() override;

private:
	void KeyCheck();
	void StateCheck();
	void AnimationCheck();

private:
	float	_speed = 5.f;
	STATE	_curState = STATE::END;
	STATE	_preState = STATE::END;
};

