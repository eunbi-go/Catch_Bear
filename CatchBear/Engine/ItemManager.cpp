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
	SetItemPosition();
}

void ItemManager::Update()
{
	CreateItem();
}

void ItemManager::LateUpdate()
{
}

void ItemManager::SetItemPosition()
{
	float x = 0.f;
	float y = 0.f;

	for (int i = 0; i < 10; ++i)
	{
		if (i > 4) y = 100.f;

		_commonPosArray[i] = Vec3((float)(x + i * 50), y, 0.f);
	}
}

void ItemManager::CreateItem()
{
	_commonItemTimer += DELTA_TIME;
	_uniqueItemTimer += DELTA_TIME;

	// 일반 아이템 생성, 30초마다 모든 종류(5) 하나씩 -> 총 6번 생성
	if (_commonItemTimer >= _commonItemTime)
	{
		if (_commonItemList.size() >= 9) return;

		shared_ptr<GameObject> item = make_shared<GameObject>();
		item->SetName(L"CommonItem");
		item->AddComponent(make_shared<Transform>());
		item->AddComponent(make_shared<Item>());

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
		_commonItemList.push_back(item);
		shared_ptr<Scene> scene = make_shared<Scene>();
		scene = GET_SINGLE(SceneManager)->GetActiveScene();
		scene->AddGameObject(item);

		_commonItemTimer = 0.f;
	}

	// 유니크 아이템 생성, 1분마다 모든 종류(2) 하나씩 -> 총 3번 생성
	if (_uniqueItemTimer >= _uniqueItemTime)
	{

		_uniqueItemTimer = 0.f;
	}

}
