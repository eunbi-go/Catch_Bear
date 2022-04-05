#include "pch.h"
#include "DashState.h"
#include "Timer.h"
#include "Player.h"
#include "AnimationController.h"
#include "Input.h"
#include "IdleState.h"
#include "DashRestState.h"
#include "GameObject.h"

PlayerState* DashState::KeyCheck(Player& player, STATE ePlayer)
{
    if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
    {
        return NULL;
    }
    else
    {
        ePlayer = STATE::DASH_REST;
        return new DashRestState(_fDashTime);
    }
    return NULL;
}

PlayerState* DashState::Update(Player& player, STATE ePlayer)
{
    // 5초 동안만 유지됨
    _fDashTime += DELTA_TIME;
    if (_fDashTime >= 5.f)
    {
        int k = 0;
        _fDashTime = 0.f;
        ePlayer = STATE::IDLE;
        return new IdleState;
    }

    return NULL;
}

void DashState::Enter(Player& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 3);
}

void DashState::End(Player& player)
{
}
