#include "pch.h"
#include "MoveState.h"
#include "Input.h"
#include "IdleState.h"
#include "Player.h"
#include "AnimationController.h"

PlayerState* MoveState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		// 현재 상태 유지
		return NULL;
	}
	else return new IdleState;
}

void MoveState::Update(Player& player)
{
}

void MoveState::Enter(Player& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 1);
}
