#include "pch.h"
#include "IdleState.h"
#include "Input.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"
#include "AnimationController.h"
#include "MoveState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "DashState.h"
#include "GameObject.h"

PlayerState* IdleState::KeyCheck(GameObject& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
		return new MoveState;
	else if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) return new JumpState;
	else if (INPUT->GetButtonDown(KEY_TYPE::CTRL))	return new AttackState;
	//else if (INPUT->GetButtonDown(KEY_TYPE::Q)) return new DashState;
	return NULL;
}

PlayerState* IdleState::Update(GameObject& player)
{
	return NULL;
}

void IdleState::Enter(GameObject& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}

void IdleState::End(GameObject& player)
{
	player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
