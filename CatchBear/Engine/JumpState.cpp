#include "pch.h"
#include "JumpState.h"
#include "Player.h"
#include "AnimationController.h"
#include "IdleState.h"

PlayerState* JumpState::KeyCheck(Player& player)
{
    return NULL;
}

PlayerState* JumpState::Update(Player& player)
{
    if (player.GetAnimationController()->IsAnimationFinish(0))
    {
        player.GetAnimationController()->SetAnimationPosition(0, 0.f);
        return new IdleState;
    }
    return NULL;
}

void JumpState::Enter(Player& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 2);
}

void JumpState::End(Player& player)
{
    player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
