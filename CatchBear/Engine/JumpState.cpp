#include "pch.h"
#include "JumpState.h"
#include "Player.h"
#include "AnimationController.h"
#include "IdleState.h"
#include "GameObject.h"

PlayerState* JumpState::KeyCheck(GameObject& player, STATE& ePlayer)
{
    return NULL;
}

PlayerState* JumpState::Update(GameObject& player, STATE& ePlayer)
{
    if (player.GetAnimationController()->IsAnimationFinish(0))
    {
        player.GetAnimationController()->SetAnimationPosition(0, 0.f);
        ePlayer = STATE::IDLE;

        return new IdleState;
    }
    return NULL;
}

void JumpState::Enter(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 2);
}

void JumpState::End(GameObject& player)
{
    player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
