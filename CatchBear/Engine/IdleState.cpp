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
#include "SlowState.h"

PlayerState* IdleState::KeyCheck(Player& player, STATE ePlayer)
{
	if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
	{
		ePlayer = STATE::WALK;
		return new MoveState;
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
	ePlayer = STATE::IDLE;
	return NULL;
}

PlayerState* IdleState::Update(Player& player, STATE ePlayer)
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
