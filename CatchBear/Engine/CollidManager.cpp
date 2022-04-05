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

	// 아이템 매니저가 리스트로 담고 있는 아이템들과 플레이어 충돌체크
	for (auto mapobj = staticObjects.begin(); mapobj != staticObjects.end(); mapobj++)
	{
		if ((*mapobj)->_boundingBox.Intersects(_player->_boundingBox))
		{
			Vec3 lk = _player->GetTransform()->GetLook();
			_player->SetIsAllowPlayerMove(false);
			break;
		}
		else
			_player->SetIsAllowPlayerMove(true);

	}
}
