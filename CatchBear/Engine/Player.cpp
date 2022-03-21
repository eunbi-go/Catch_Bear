#include "pch.h"
#include "Player.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CameraScript.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::LateUpdate()
{
	KeyCheck();
}

void Player::KeyCheck()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 rot = GetTransform()->GetLocalRotation();

	// 이동
	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	// 회전
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		rot.y -= DELTA_TIME * _rotSpeed;
		GetTransform()->SetLocalRotation(rot);
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		rot.y += DELTA_TIME * _rotSpeed;
		GetTransform()->SetLocalRotation(rot);
	}


	//if (INPUT->GetButton(KEY_TYPE::A))
	//	pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::D))
	//	pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	GetTransform()->SetLocalPosition(pos);

	// 현재 씬에서 카메라(Main_Camera)를 가져온다.
	// 카메라가 가지고 있는 스크립트(CameraScript)에서 Followlayer()를 실행시킨다.

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_player = GetGameObject();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Main_Camera")
		{
			_camera = gameObject;
			break;
		}
	}

	shared_ptr<CameraScript> cameraScript = make_shared<CameraScript>();
	cameraScript = static_pointer_cast<CameraScript>(_camera->GetScript(0));
	cameraScript->GetTransform()->SetParent(_player->GetTransform());
	cameraScript->FollowPlayer(_player);
}
