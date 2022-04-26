#include "pch.h"
#include "SlowState.h"
#include "Input.h"
#include "Player.h"
#include "AnimationController.h"
#include "DashRestState.h"
#include "Timer.h"
#include "IdleState.h"
#include "SlowRestState.h"
#include "GameObject.h"

PlayerState* SlowState::KeyCheck(GameObject& player, STATE& ePlayer)
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

PlayerState* SlowState::Update(GameObject& player, STATE& ePlayer)
{
    // 5초 동안만 유지됨
    _fTime += DELTA_TIME;

    if (_fTime < 5.f)
    {
        _fSlowSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerSlowSpeed();
        static_pointer_cast<Player>(player.GetScript(0))->SetPlayerSpeed(_fSlowSpeed);
        ePlayer = STATE::SLOW;

        //cout << "Slow 시작" << endl;
        //float playerSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerSpeed();
        //cout << "Player Speed: " << playerSpeed << endl;
    }

    else if (_fTime >= 5.f)
    {
        float fOriginalSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerOriginalSpeed();
        static_pointer_cast<Player>(player.GetScript(0))->SetPlayerSpeed(10.f);
        static_pointer_cast<Player>(player.GetScript(0))->SetCurItem(Player::ITEM::SPEED_DOWN, false);
        _fTime = 0.f;
        ePlayer = STATE::IDLE;

        //cout << "Slow 끝" << endl;
        //float playerSpeed = static_pointer_cast<Player>(player.GetScript(0))->GetPlayerSpeed();
        //cout << "Player Speed: " << playerSpeed << endl;

        return new IdleState;
    }

    return NULL;
}

void SlowState::Enter(GameObject& player)
{
    player.GetAnimationController()->SetTrackAnimationSet(0, 1);
    player.GetAnimationController()->SetTrackSpeed(0, 0.2f);
}

void SlowState::End(GameObject& player)
{
    _fTime = 0.f;

    player.GetAnimationController()->SetAnimationPosition(0, 0.f);
    player.GetAnimationController()->SetTrackSpeed(0, 1.f);
}
