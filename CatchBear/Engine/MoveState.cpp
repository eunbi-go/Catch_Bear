#include "pch.h"
#include "MoveState.h"
#include "Input.h"
#include "IdleState.h"
#include "Player.h"
#include "AnimationController.h"
#include "JumpState.h"
#include "AttackState.h"

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
