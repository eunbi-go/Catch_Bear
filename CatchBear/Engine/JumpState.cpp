#include "pch.h"
#include "JumpState.h"
#include "Player.h"
#include "AnimationController.h"
#include "IdleState.h"
#include "GameObject.h"

PlayerState* JumpState::KeyCheck(GameObject& player)
{
    return NULL;
}

void JumpState::Update(GameObject& player)
{
    if (player.GetAnimationController()->IsAnimationFinish(0))
    {
        player.GetAnimationController()->SetTrackAnimationSet(0, 0.f);
        //return new IdleState;
    }
    //return NULL;
}

void JumpState::Enter(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 2);
}

void JumpState::End(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 0.f);
}
