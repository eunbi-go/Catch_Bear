#include "pch.h"
#include "MoveState.h"
#include "Input.h"
#include "IdleState.h"
#include "Player.h"
#include "AnimationController.h"
#include "JumpState.h"
#include "AttackState.h"
#include "Timer.h"

PlayerState* MoveState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		// 현재 상태 유지
		return NULL;
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) return new JumpState;
	else if (INPUT->GetButtonDown(KEY_TYPE::CTRL))	return new AttackState;
	else return new IdleState;
}

PlayerState* MoveState::Update(Player& player)
{
	// 플레이어가 속도 감소 상태일때
	if (player.GetCurItem(Player::ITEM::SPEED_DOWN))
	{
		_fSpeedDownTime += DELTA_TIME;

		if (_fSpeedDownTime <= 5.f)
		{
			player.SetPlayerSpeed(_fSlowSpeed);
		}

		else
		{
			player.SetPlayerSpeed(player.GetPlayerOriginalSpeed());
			_fSpeedDownTime = 0.f;
			player.SetCurItem(Player::ITEM::SPEED_DOWN, false);
		}

	}

	return NULL;
}

void MoveState::Enter(Player& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 1);
}

void MoveState::End(Player& player)
{
	player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
