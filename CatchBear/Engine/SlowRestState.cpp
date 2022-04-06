#include "pch.h"
#include "SlowRestState.h"
#include "Input.h"
#include "Player.h"
#include "AnimationController.h"
#include "SlowState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Timer.h"
#include "IdleState.h"

PlayerState* SlowRestState::KeyCheck(GameObject& player, STATE ePlayer)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		ePlayer = STATE::WALK_DOWN;
		return new SlowState(_fTime);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		ePlayer = STATE::JUMP;
		return new JumpState;
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::CTRL))
	{
		ePlayer = STATE::ATTACK;
		return new AttackState;
	}
	return NULL;
}

PlayerState* SlowRestState::Update(GameObject& player, STATE ePlayer)
{
	_fTime += DELTA_TIME;
	if (_fTime >= 5.f)
	{
		ePlayer = STATE::IDLE;
		return new IdleState;
	}
	return nullptr;
}

void SlowRestState::Enter(GameObject& player)
{
}

void SlowRestState::End(GameObject& player)
{
}
