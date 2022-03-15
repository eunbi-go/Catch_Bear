#include "pch.h"
#include "CameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"

CameraScript::CameraScript()
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::LateUpdate()
{
	//if (INPUT->GetButton(KEY_TYPE::W))
	//	pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::S))
	//	pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::A))
	//	pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::D))
	//	pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::Q))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x += DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::E))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x -= DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::Z))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y += DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::C))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y -= DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}
}

void CameraScript::FollowPlayer(shared_ptr<GameObject> player)
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	//shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	//const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	//for (auto& gameObject : gameObjects)
	//{
	//	if (gameObject->GetName() == L"Player")
	//	{
	//		_testPlayer = gameObject;
	//		pos = _testPlayer->GetTransform()->GetLocalPosition();
	//		pos -= Vec3(0.f, 0.f, 300.f);
	//		break;
	//	}
	//}

	pos = player->GetTransform()->GetLocalPosition();
	pos -= Vec3(0.f, 0.f, 300.f);

	GetTransform()->SetLocalPosition(pos);

}
