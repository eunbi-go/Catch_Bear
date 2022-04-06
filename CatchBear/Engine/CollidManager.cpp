#include "pch.h"
#include "CollidManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"

void CollidManager::Update()
{
	ColiisionPlayerToStaticObj();
}

void CollidManager::ColiisionPlayerToStaticObj()
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	shared_ptr<GameObject>	_player = make_shared<GameObject>();

	// 씬 안의 플레이어 찾기
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player")
		{
			_player = gameObject;
			break;
		}
	}

	auto& staticObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetStaticObj();

	for (auto mapobj = staticObjects.begin(); mapobj != staticObjects.end(); mapobj++)
	{
		if ((*mapobj)->_boundingBox.Intersects(_player->_boundingBox))
		{
			Vec3 lk = _player->GetTransform()->GetLook();
			float ox = (*mapobj)->GetTransform()->GetLocalPosition().x;
			float oz = (*mapobj)->GetTransform()->GetLocalPosition().z;
			_player->SetIsAllowPlayerMove(false);
			break;
		}
		else
			_player->SetIsAllowPlayerMove(true);

	}
}
