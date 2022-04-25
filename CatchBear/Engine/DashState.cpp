#include "pch.h"
#include "DashState.h"
#include "Timer.h"
#include "Player.h"
#include "AnimationController.h"
#include "Input.h"
#include "IdleState.h"
#include "DashRestState.h"
#include "GameObject.h"
#include "ServerSession.h"

PlayerState* DashState::KeyCheck(GameObject& player, STATE& ePlayer)
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

PlayerState* DashState::Update(GameObject& player, STATE& ePlayer)
{
    // 5초 동안만 유지됨
    _fDashTime += DELTA_TIME;

    if (_fDashTime <= 5.f)
    {
        _fDashSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerDashSpeed();
        static_pointer_cast<Player>(player.GetScript(0))->SetPlayerSpeed(_fDashSpeed);
        ePlayer = STATE::DASH;

        //float playerSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerSpeed();
        //cout << "Player Speed: " << playerSpeed << endl;
    }

    else if (_fDashTime > 5.f)
    {
        float fOriginalSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerOriginalSpeed();
        static_pointer_cast<Player>(player.GetScript(0))->SetPlayerSpeed(10.f);
        static_pointer_cast<Player>(player.GetScript(0))->SetCurItem(Player::ITEM::SPEED_UP, false);
        _fDashTime = 0.f;
        ePlayer = STATE::IDLE;

        //float playerSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerSpeed();
        //cout << "Player Speed: " << playerSpeed << endl;

        return new IdleState;
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
