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

void ItemManager::Init()
{
	_itemArray = { _commonItemList, _uniqueItemList, _treasureList };
	SetItemPosition();
}

void ItemManager::Update()
{
	CreateCommonItem();
	CreateUniqueItem();
	CreateTreasure();
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
		if (i > 4) y = 100.f;

		_commonPosArray[i] = Vec3((float)(x + i * 50), y, 0.f);
	}
}

void ItemManager::CreateCommonItem()
{
	_commonItemTimer += DELTA_TIME;

	// 일반 아이템 생성, 30초마다 모든 종류(5) 하나씩 -> 총 6번 생성
	if (_commonItemTimer >= _commonItemTime)
	{
		int idx = 0;

		for (int i = 0; i < 5; ++i)
		{
			if (_commonItemList.size() >= 5) break;

			shared_ptr<GameObject> item = make_shared<GameObject>();
			item->SetName(L"CommonItem");
			item->AddComponent(make_shared<Transform>());
			item->AddComponent(make_shared<Item>());

			// 좌표값 임시로 넣은거
			Vec3 pos = _commonPosArray[index++];
			item->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			item->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			item->GetTransform()->SetLocalPosition(pos);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
				meshRenderer->SetMaterial(material->Clone());
			}
			item->AddComponent(meshRenderer);

			// enum값 설정 - ItemType, ItemEffect
			// Item이 가지고 있는 스크립트 Item 하나뿐, GetScript(0)은 Item 스크립트임
			static_pointer_cast<Item>(item->GetScript(0))->SetItemType(ITEM_TYPE::COMMON);
			switch (idx)
			{
			case 1:
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect(ITEM_EFFECT::SPEED_UP);
				break;
			case 2:
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect(ITEM_EFFECT::TELEPORT);
				break;
			case 3:
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect(ITEM_EFFECT::SHIELD);
				break;
			case 4:
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect(ITEM_EFFECT::SPEED_DOWN);
				break;
			case 5:
				static_pointer_cast<Item>(item->GetScript(0))->SetItemEffect(ITEM_EFFECT::BLIND);
				break;

				idx++;
			}

			_commonItemList.push_back(item);

			shared_ptr<Scene> scene = make_shared<Scene>();
			scene = GET_SINGLE(SceneManager)->GetActiveScene();
			scene->AddGameObject(item);
		}

		_commonItemTimer = 0.f;
	}
}

void ItemManager::CreateUniqueItem()
{
	_uniqueItemTimer += DELTA_TIME;

	// 유니크 아이템 생성, 1분마다 모든 종류(2) 하나씩 -> 총 3번 생성
	if (_uniqueItemTimer >= _uniqueItemTime)
	{

		_uniqueItemTimer = 0.f;
	}
}

void ItemManager::CreateTreasure()
{
}
