#include "pch.h"
#include "Player.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "AnimationController.h"
#include "AnimationTrack.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::LateUpdate()
{
	//GetTransform()->PreRender();
	KeyCheck();
	StateCheck();
	AnimationCheck();

	GetAnimationController()->AdvanceTime(DELTA_TIME);
	GetTransform()->UpdateTransform(NULL);
	GetAnimationController()->SetWorldMatrix();
}

void Player::KeyCheck()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (_curState != JUMP)
	{
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			_curState = WALK;
			pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		}

		else if (INPUT->GetButton(KEY_TYPE::S))
		{
			_curState = WALK;
			pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		}

		else if (INPUT->GetButton(KEY_TYPE::A))
		{
			_curState = WALK;
			pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		}

		else if (INPUT->GetButton(KEY_TYPE::D))
		{
			_curState = WALK;
			pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		}
		else	_curState = IDLE;
	}

	if (INPUT->GetButtonUp(KEY_TYPE::SPACE))
	{
		_curState = JUMP;
	}

	GetTransform()->SetLocalPosition(pos);

}

void Player::StateCheck()
{
	if (_curState != _preState)
	{
		switch (_curState)
		{
		case Player::IDLE:
			GetAnimationController()->SetTrackAnimationSet(0, 0);
			break;
		case Player::WALK:
			GetAnimationController()->SetTrackAnimationSet(0, 1);
			break;
		case Player::DASH:
			GetAnimationController()->SetTrackAnimationSet(0, 3);
			break;
		case Player::JUMP:
			GetAnimationController()->SetTrackAnimationSet(0, 2);
			break;
		case Player::ATTACK:
			GetAnimationController()->SetTrackAnimationSet(0, 4);
			break;
		case Player::END:
			break;
		default:
			break;
		}

		_preState = _curState;
	}
}

void Player::AnimationCheck()
{
	if (_curState == DASH || _curState == JUMP || _curState == ATTACK)
	{
		if (GetAnimationController()->IsAnimationFinish(0))
			_curState = IDLE;
	}
}
