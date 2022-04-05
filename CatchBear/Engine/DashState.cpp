#include "pch.h"
#include "DashState.h"
#include "Timer.h"
#include "Player.h"
#include "AnimationController.h"
#include "Input.h"
#include "IdleState.h"
#include "DashRestState.h"

PlayerState* DashState::KeyCheck(Player& player)
{
    if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
    {
        return NULL;
    }
    else return new DashRestState(_fDashTime);

    return NULL;
}

PlayerState* DashState::Update(Player& player)
{
    // 5초 동안만 유지됨
    _fDashTime += DELTA_TIME;
    if (_fDashTime >= 5.f)
    {
        int k = 0;
        player.SetPlayerSpeed(_fOriginalSpeed);
        player.SetCurItem(Player::ITEM::SPEED_UP, false);
        _fDashTime = 0.f;
        return new IdleState;
    }

    else if (_fDashTime < 5.f)
    {
        player.SetPlayerSpeed(_fDashSpeed);
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
