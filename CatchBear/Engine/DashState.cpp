#include "pch.h"
#include "DashState.h"
#include "Timer.h"
#include "Player.h"
#include "AnimationController.h"
#include "Input.h"
#include "IdleState.h"
#include "DashRestState.h"
#include "GameObject.h"

PlayerState* DashState::KeyCheck(GameObject& player)
{
    if (INPUT->GetButton(KEY_TYPE::UP) || INPUT->GetButton(KEY_TYPE::DOWN))
    {
        return NULL;
    }
    else return new DashRestState(_fDashTime);

    return NULL;
}

PlayerState* DashState::Update(GameObject& player)
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

void DashState::Enter(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 3);
}

void DashState::End(GameObject& player)
{
}
