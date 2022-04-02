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

PlayerState* DashRestState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
		return new DashState(_fDashTime);
	else if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) return new JumpState;
	else if (INPUT->GetButtonDown(KEY_TYPE::CTRL))	return new AttackState;
	return NULL;
}

PlayerState* DashRestState::Update(Player& player)
{
	_fDashTime += DELTA_TIME;
	if (_fDashTime >= 5.f)
		return new IdleState;
    return nullptr;
}

void DashRestState::Enter(Player& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}

void DashRestState::End(Player& player)
{
}
