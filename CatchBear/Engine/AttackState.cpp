#include "pch.h"
#include "AttackState.h"
#include "Player.h"
#include "AnimationController.h"
#include "Input.h"
#include "IdleState.h"
#include "GameObject.h"

PlayerState* AttackState::KeyCheck(Player& player, STATE ePlayer)
{
    return nullptr;
}

PlayerState* AttackState::Update(Player& player, STATE ePlayer)
{
    if (player.GetAnimationController()->IsAnimationFinish(0))
    {
        player.GetAnimationController()->SetAnimationPosition(0, 0.f);
        ePlayer = STATE::IDLE;
        return new IdleState;
    }
    return NULL;
}

void AttackState::Enter(Player& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 4);
}

void AttackState::End(Player& player)
{
    player.GetAnimationController()->SetAnimationPosition(0, 0.f);
}
