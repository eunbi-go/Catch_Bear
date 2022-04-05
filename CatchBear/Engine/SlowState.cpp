#include "pch.h"
#include "SlowState.h"
#include "Input.h"
#include "Player.h"
#include "AnimationController.h"
#include "DashRestState.h"
#include "Timer.h"
#include "IdleState.h"
#include "SlowRestState.h"

PlayerState* SlowState::KeyCheck(Player& player, STATE ePlayer)
{
    if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
    {
        return NULL;
    }
    else
    {
        ePlayer = STATE::SLOW_REST;
        return new SlowRestState(_fTime);
    }
    return NULL;
}

PlayerState* SlowState::Update(Player& player, STATE ePlayer)
{
    // 5초 동안만 유지됨
    _fTime += DELTA_TIME;
    if (_fTime >= 5.f)
    {
        _fTime = 0.f;
        ePlayer = STATE::IDLE;
        return new IdleState;
    }

    return NULL;
}

void SlowState::Enter(Player& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 1);
    player.GetAnimationController()->SetTrackSpeed(0, 0.2f);
}

void SlowState::End(Player& player)
{
    player.GetAnimationController()->SetAnimationPosition(0, 0.f);
    player.GetAnimationController()->SetTrackSpeed(0, 1.f);
}
