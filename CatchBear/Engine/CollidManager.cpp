#include "pch.h"
#include "CollidManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "ServerSession.h"

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
		if (gameObject->GetName() == L"Player" && gameObject->GetPlayerID() == mysession->GetPlayerID())
		{
			_player = gameObject;
			break;
		}
	}

	auto& staticObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetStaticObj();

	for (auto mapobj = staticObjects.begin(); mapobj != staticObjects.end(); mapobj++)
	{
		if ((*mapobj)->GetName() == L"Tree_01" || (*mapobj)->GetName() == L"Tree_02" ||
			(*mapobj)->GetName() == L"Tree_03" || (*mapobj)->GetName() == L"Rock_02" ||
			(*mapobj)->GetName() == L"Rock_03" || (*mapobj)->GetName() == L"Fence_Type1_02")
		{
			if ((*mapobj)->GetBoundingBox().Intersects(_player->GetBoundingBox()))
			{
				_player->SetIsAllowPlayerMove(false);
				break;
			}
			else
				_player->SetIsAllowPlayerMove(true);
		}

	}
}

