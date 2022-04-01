#include "pch.h"
#include "PlayerState.h"
#include "Input.h"
#include "Player.h"
#include "IdleState.h"

void PlayerState::KeyCheck(Player& player)
{
	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		player._state = &PlayerState::idle;
	}
}
