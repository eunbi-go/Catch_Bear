#include "pch.h"
#include "Item.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Material.h"
#include "SceneManager.h"
#include "Scene.h"

Item::Item()
{
}

Item::Item(ITEM itemType)
{
	_itemType = itemType;
}

Item::~Item()
{
}

void Item::Update()
{
}

void Item::CreateItem()
{
	// 현재 씬 가져오기
	shared_ptr<Scene> scene = make_shared<Scene>();
	scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// 일단 큐브로 생성
	shared_ptr<GameObject> item = make_shared<GameObject>();
	item->SetName(L"Item");
	item->AddComponent(make_shared<Transform>());
	item->AddComponent(make_shared<Item>());
	item->GetTransform()->SetLocalScale(Vec3(25.f, 25.f, 25.f));
	item->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
	item->GetTransform()->SetLocalPosition(Vec3(0, 0.f, 0.f));
	item->SetStatic(false);
	item->SetCheckFrustum(false);

	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
	meshRenderer->SetMesh(sphereMesh);

	shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
	meshRenderer->SetMaterial(material->Clone());

	item->AddComponent(meshRenderer);
	scene->AddGameObject(item);
}
