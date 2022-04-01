#include "pch.h"
#include "IdleState.h"
#include "Input.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"
#include "AnimationController.h"

void IdleState::KeyCheck(shared_ptr<Player> player)
{
	Vec3 pos = player->GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos += player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
		//player->GetTransform()->SetLocalPosition(pos);
		//
		//player->_state = PlayerState::idle;
		//player->GetAnimationController()->SetTrackAnimationSet(0, 1);
	}

	//if (INPUT->GetButton(KEY_TYPE::DOWN))
	//{
	//	pos -= player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
	//	player->GetTransform()->SetLocalPosition(pos);

	//	player->GetAnimationController()->SetTrackAnimationSet(0, 1);
	//}

	//if (INPUT->GetButton(KEY_TYPE::RIGHT))
	//{
	//	pos += player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
	//	player->GetTransform()->SetLocalPosition(pos);

	//	player->GetAnimationController()->SetTrackAnimationSet(0, 1);
	//}

	//if (INPUT->GetButton(KEY_TYPE::LEFT))
	//{
	//	pos += player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
	//	player->GetTransform()->SetLocalPosition(pos);

	//	player->GetAnimationController()->SetTrackAnimationSet(0, 1);
	//}
}

void IdleState::Update(shared_ptr<Player> player)
{
}
