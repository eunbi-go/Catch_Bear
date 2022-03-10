#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"

PlayerScript::PlayerScript()
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::LateUpdate()
{
	KeyCheck();
}

void PlayerScript::KeyCheck()
{
	Vec3 pos = GetTransform()->GetLocalPosition();


	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;


	GetTransform()->SetLocalPosition(pos);

}
