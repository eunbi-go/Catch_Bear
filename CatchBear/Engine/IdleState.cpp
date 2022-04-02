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
		return new MoveState();
	return NULL;
}

void IdleState::Update(GameObject& player)
{
}

void IdleState::Enter(GameObject& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}

void IdleState::End(GameObject& player)
{
}
