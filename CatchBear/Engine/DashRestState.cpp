#include "pch.h"
#include "DashRestState.h"
#include "Input.h"
#include "DashState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Player.h"
#include "AnimationController.h"
#include "Timer.h"
#include "IdleState.h"
#include "GameObject.h"

PlayerState* DashRestState::KeyCheck(GameObject& player, STATE& ePlayer)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		ePlayer = STATE::DASH;
		return new DashState(_fDashTime);
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

PlayerState* DashRestState::Update(GameObject& player, STATE ePlayer)
{
	_fDashTime += DELTA_TIME;
	if (_fDashTime >= 5.f)
	{
		ePlayer = STATE::IDLE;
		return new IdleState;
	}
    return nullptr;
}

void DashRestState::Enter(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}

void DashRestState::End(GameObject& player)
{
}
