#include "pch.h"
#include "MoveState.h"
#include "Input.h"
#include "IdleState.h"
#include "Player.h"
#include "AnimationController.h"
#include "JumpState.h"
#include "AttackState.h"
#include "GameObject.h"
#include "Timer.h"

PlayerState* MoveState::KeyCheck(GameObject& player, STATE& ePlayer)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		// 현재 상태 유지
		ePlayer = STATE::WALK;
		return NULL;
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
	else
	{
		ePlayer = STATE::IDLE;
		return new IdleState;
	}
}

PlayerState* MoveState::Update(GameObject& player, STATE& ePlayer)
{
	// 플레이어가 속도 감소 상태일때

	////static_pointer_cast<Player>(player.GetScript(0))->GetCurItem(Player::ITEM::SPEED_DOWN)

	//if (static_pointer_cast<Player>(player.GetScript(0))->GetCurItem(Player::ITEM::SPEED_DOWN))
	//{
	//	_fSpeedDownTime += DELTA_TIME;

	//	if (_fSpeedDownTime <= 5.f)
	//	{
	//		player.SetPlayerSpeed(_fSlowSpeed);
	//	}

	//	else
	//	{
	//		player.SetPlayerSpeed(player.GetPlayerOriginalSpeed());
	//		_fSpeedDownTime = 0.f;
	//		player.SetCurItem(Player::ITEM::SPEED_DOWN, false);
	//	}

	//}

	ePlayer = STATE::WALK;
	return NULL;
}

void MoveState::Enter(GameObject& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 1);
}

void MoveState::End(GameObject& player)
{
	player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
