#include "pch.h"
#include "ItemManager.h"
#include "Item.h"
#include "Scene.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Scene.h"
#include "SceneManager.h"
#include "MeshData.h"

void ItemManager::Init()
{
	_itemArray = { _commonItemList, _uniqueItemList, _treasureList };

	_commonItemMesh = GET_SINGLE(Resources)->LoadFBX(L"present1.bin");	// common
	_uniqueItemMesh = GET_SINGLE(Resources)->LoadFBX(L"present4.bin");	// unique

	SetItemPosition();
}

void ItemManager::Update()
{
	CreateCommonItem();
	CreateUniqueItem();
	CreateTreasure();

	Collision_ItemToPlayer();
}

void ItemManager::LateUpdate()
{
}

void ItemManager::SetItemPosition()
{
	// 좌표 회의때 정하기
	float x = 0.f;
	float y = 0.f;

	for (int i = 0; i < 10; ++i)
	{
		_commonPosArray[i] = Vec3((float)(x + i * 20), y, 0.f);
	}

	for (int i = 0; i < 10; ++i)
	{
		_uniquePosArray[i] = Vec3((float)(x + i * 20), y, 30.f);
	}
}

void ItemManager::CreateCommonItem()
{
	// 일반 아이템 생성, 30초마다 모든 종류(5개) 하나씩 생성 -> 총 6번 생성
	_commonItemTimer += DELTA_TIME;

	if (_commonItemTimer >= _commonItemTime)	// test 5초
	{	
		for (int i = 0; i < 5; ++i)
		{
			vector<shared_ptr<GameObject>>	commonItem = _commonItemMesh->Instantiate();

			for (auto& item : commonItem)
			{
				item->SetName(L"CommonItem");
				item->SetCheckFrustum(false);

				// 임시 좌표값
				if (_commonIndex > 9) _commonIndex = 0;
				Vec3 pos = _commonPosArray[_commonIndex++];
				item->GetTransform()->SetLocalPosition(pos);
				item->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
				item->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				item->_boundingExtents = XMFLOAT3(5.f, 5.f, 5.f);
				item->_boundingBox = BoundingOrientedBox(
					XMFLOAT3(0.0f, 0.0f, 0.0f), item->_boundingExtents, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
				item->AddComponent(make_shared<Item>());

				// Item enum값 설정 - ItemType, ItemEffect
				static_pointer_cast<Item>(item->GetScript(0))->SetItemType(ITEM_TYPE::COMMON);
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect((ITEM_EFFECT)i);
				_commonItemList.push_back(item);

				shared_ptr<Scene> scene = make_shared<Scene>();
				scene = GET_SINGLE(SceneManager)->GetActiveScene();
				scene->AddGameObject(item);
			}
		}
		_commonItemTimer = 0.f;
	}
}

void ItemManager::CreateUniqueItem()
{
	// 유니크 아이템 생성, 1분마다 모든 종류(2) 하나씩 -> 총 3번 생성
	_uniqueItemTimer += DELTA_TIME;

	if (_uniqueItemTimer >= _uniqueItemTime)	// test 10초
	{
		for (int i = 0; i < 2; ++i)
		{
			vector<shared_ptr<GameObject>>	uniqueItem = _uniqueItemMesh->Instantiate();

			for (auto& item : uniqueItem)
			{
				item->SetName(L"UniqueItem");
				item->SetCheckFrustum(false);

				// 임시 좌표값
				if (_uniqueIndex > 9) _uniqueIndex = 0;
				Vec3 pos = _uniquePosArray[_uniqueIndex++];
				item->GetTransform()->SetLocalPosition(pos);
				item->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
				item->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				item->_boundingExtents = XMFLOAT3(5.f, 5.f, 5.f);
				item->_boundingBox = BoundingOrientedBox(
					XMFLOAT3(0.0f, 0.0f, 0.0f), item->_boundingExtents, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
				item->AddComponent(make_shared<Item>());

				// Item enum값 설정 - ItemType, ItemEffect
				static_pointer_cast<Item>(item->GetScript(0))->SetItemType(ITEM_TYPE::UNIQUE);
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect((ITEM_EFFECT)(i + 5));
				_uniqueItemList.push_back(item);

				shared_ptr<Scene> scene = make_shared<Scene>();
				scene = GET_SINGLE(SceneManager)->GetActiveScene();
				scene->AddGameObject(item);
			}
		}
		_uniqueItemTimer = 0.f;
	}

}

void ItemManager::CreateTreasure()
{
}

void ItemManager::Collision_ItemToPlayer()
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	// 씬 안의 플레이어 찾기
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player")
		{
			_player = gameObject;
			break;
		}
	}

	// 아이템 매니저가 리스트로 담고 있는 아이템들과 플레이어 충돌체크
	for (auto item = _commonItemList.begin(); item != _commonItemList.end(); item++)
	{
		if ((*item)->_boundingBox.Intersects(_player->_boundingBox))
		{
			int a = 0;
		}

		// else item++;
	}

}
