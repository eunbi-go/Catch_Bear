#include "pch.h"
#include "IdleState.h"
#include "Input.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"
#include "AnimationController.h"
#include "MoveState.h"
#include "JumpState.h"

PlayerState* IdleState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
		return new MoveState();
	else if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) return new JumpState;
	return NULL;
}

PlayerState* IdleState::Update(Player& player)
{
	return NULL;
}

void IdleState::Enter(Player& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}

void IdleState::End(Player& player)
{
	player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
