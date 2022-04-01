#include "pch.h"
#include "IdleState.h"
#include "Input.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"
#include "AnimationController.h"
#include "MoveState.h"

PlayerState* IdleState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
		return new MoveState();
	return NULL;
}

void IdleState::Update(Player& player)
{
}

void IdleState::Enter(Player& player)
{
	player.GetAnimationController()->SetTrackAnimationSet(0, 0);
}
