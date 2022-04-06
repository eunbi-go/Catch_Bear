#include "pch.h"
#include "MoveState.h"
#include "Input.h"
#include "IdleState.h"
#include "Player.h"
#include "AnimationController.h"
#include "JumpState.h"
#include "AttackState.h"
#include "GameObject.h"

PlayerState* MoveState::KeyCheck(GameObject& player, STATE ePlayer)
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

PlayerState* MoveState::Update(GameObject& player, STATE ePlayer)
{
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
