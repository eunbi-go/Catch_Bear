#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "CameraScript.h"
#include "Resources.h"
#include "ParticleSystem.h"
#include "Player.h"
#include "MeshData.h"
#include "CharacterData.h"
#include "AnimationController.h"
#include "AnimationTrack.h"

shared_ptr<Scene> scene = make_shared<Scene>();

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}

void SceneManager::MakePlayer(uint64 _playerID)
{
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"Player");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<Player>());
		obj->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
		obj->GetTransform()->SetLocalPosition(Vec3(200.f + (_playerID * 100.f), 0.f, 500.f));
		obj->SetStatic(false);
		obj->SetCheckFrustum(false);	// 컬링 오류나서 컬링하지 않도록 설정해둠
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
			meshRenderer->SetMaterial(material->Clone());
		}

		// 플레이어ID 설정
		obj->SetPlayerID(_playerID);

		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
		scene->AddPlayers(_playerID, obj);
	}
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion

	

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45도
		camera->AddComponent(make_shared<CameraScript>());
		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		scene->AddGameObject(camera);
	}
#pragma endregion

//#pragma region TestPlayer
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetName(L"Player");
//		obj->AddComponent(make_shared<Transform>());
//		obj->AddComponent(make_shared<Player>());
//		obj->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(200, 0.f, 500.f));
//		obj->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(0, 0.f, 0.f));
//
//		obj->SetStatic(false);
//		obj->SetCheckFrustum(false);	// 컬링 오류나서 컬링하지 않도록 설정해둠
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
//			meshRenderer->SetMaterial(material->Clone());
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion

#pragma region 테스트용 Object
	{
		//shared_ptr<GameObject> obj = make_shared<GameObject>();
		//obj->SetName(L"Wall");
		//obj->AddComponent(make_shared<Transform>());
		//obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		//obj->GetTransform()->SetLocalPosition(Vec3(0, 0.f, 700.f));
		//obj->SetStatic(false);
		//shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		//{
		//	shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		//	meshRenderer->SetMesh(sphereMesh);
		//}
		//{
		//	shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
		//	meshRenderer->SetMaterial(material->Clone());
		//}
		//obj->AddComponent(meshRenderer);
		//scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky", L"..\\Resources\\Texture\\Skybox.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region StaticMesh
	LoadMapObjects(scene);
#pragma endregion

#pragma region TestPlayer
	{
		shared_ptr<CharacterData> CharacData = GET_SINGLE(Resources)->LoadCharacter(L"EvilbearL2.bin");
		
		vector<shared_ptr<GameObject>>	gameObjects = CharacData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Player");
			gameObject->GetTransform()->SetLocalPosition(Vec3(15.f, -10.f, 50.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->AddComponent(make_shared<Player>());
			gameObject->GetAnimationController()->SetTrackAnimationSet(0, 0);
			gameObject->SetStatic(false);
			gameObject->SetCheckFrustum(false);	// 컬링 오류나서 컬링하지 않도록 설정해둠
			scene->AddGameObject(gameObject);
		}
	}
#pragma endregion

#pragma region Terrain
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(1000.f, 1000.f, 50.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-500.f, -50.f, 0.f));
		obj->SetStatic(true);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadTerrainMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Terrain");
			material->SetInt(0, 0);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		//obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 0));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion

	//
//#pragma region Point Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
//		light->GetLight()->SetLightRange(200.f);
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//
//#pragma region Spot Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
//		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
//		light->GetLight()->SetLightRange(200.f);
//		light->GetLight()->SetLightAngle(3.14f / 2);
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//


	return scene;
}

void SceneManager::LoadMapObjects(shared_ptr<Scene> scene)
{
	//// Tree_01
	//shared_ptr<MeshData> meshTree01 = GET_SINGLE(Resources)->LoadFBX(L"Tree_01.bin");

	//vector<shared_ptr<GameObject>>	objectsTree01 = meshTree01->Instantiate();

	//for (auto& gameObject : objectsTree01)
	//{
	//	gameObject->SetName(L"Tree01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(0.03f, 0.03f, 0.03f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Tree_02
	//shared_ptr<MeshData> meshTree02 = GET_SINGLE(Resources)->LoadFBX(L"Tree_02.bin");

	//vector<shared_ptr<GameObject>>	objectsTree02 = meshTree02->Instantiate();

	//for (auto& gameObject : objectsTree02)
	//{
	//	gameObject->SetName(L"Tree02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Tree_03
	//shared_ptr<MeshData> meshTree03 = GET_SINGLE(Resources)->LoadFBX(L"Tree_03.bin");

	//vector<shared_ptr<GameObject>>	objectsTree03 = meshTree03->Instantiate();

	//for (auto& gameObject : objectsTree03)
	//{
	//	gameObject->SetName(L"Tree03");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Mushroom_01
	//shared_ptr<MeshData> meshMushroom01 = GET_SINGLE(Resources)->LoadFBX(L"Mushroom_01.bin");

	//vector<shared_ptr<GameObject>>	objectsMushromm01 = meshMushroom01->Instantiate();

	//for (auto& gameObject : objectsMushromm01)
	//{
	//	gameObject->SetName(L"Mushroom01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 30.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Mushroom_02
	//shared_ptr<MeshData> meshMushroom02 = GET_SINGLE(Resources)->LoadFBX(L"Mushroom_02.bin");

	//vector<shared_ptr<GameObject>>	objectsMushromm02 = meshMushroom02->Instantiate();

	//for (auto& gameObject : objectsMushromm02)
	//{
	//	gameObject->SetName(L"Mushroom02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 30.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Rock_01
	//shared_ptr<MeshData> meshRock01 = GET_SINGLE(Resources)->LoadFBX(L"Rock_01.bin");

	//vector<shared_ptr<GameObject>>	objectsRock01 = meshRock01->Instantiate();

	//for (auto& gameObject : objectsRock01)
	//{
	//	gameObject->SetName(L"Rock01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 10.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Rock_02
	//shared_ptr<MeshData> meshRock02 = GET_SINGLE(Resources)->LoadFBX(L"Rock_02.bin");

	//vector<shared_ptr<GameObject>>	objectsRock02 = meshRock02->Instantiate();

	//for (auto& gameObject : objectsRock02)
	//{
	//	gameObject->SetName(L"Rock02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Rock_03
	//shared_ptr<MeshData> meshRock03 = GET_SINGLE(Resources)->LoadFBX(L"Rock_03.bin");

	//vector<shared_ptr<GameObject>>	objectsRock03 = meshRock03->Instantiate();

	//for (auto& gameObject : objectsRock03)
	//{
	//	gameObject->SetName(L"Rock03");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Rock_04
	//shared_ptr<MeshData> meshRock04 = GET_SINGLE(Resources)->LoadFBX(L"Rock_04.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsRock04 = meshRock04->Instantiate();
	//
	//for (auto& gameObject : objectsRock04)
	//{
	//	gameObject->SetName(L"Rock04");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Stump_01
	//shared_ptr<MeshData> meshStump01 = GET_SINGLE(Resources)->LoadFBX(L"Stump_01.bin");

	//vector<shared_ptr<GameObject>>	objectsStump01 = meshStump01->Instantiate();

	//for (auto& gameObject : objectsStump01)
	//{
	//	gameObject->SetName(L"Stump01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Grass_01
	//shared_ptr<MeshData> meshGrass01 = GET_SINGLE(Resources)->LoadFBX(L"Grass_01.bin");

	//vector<shared_ptr<GameObject>>	objectsGrass01 = meshGrass01->Instantiate();

	//for (auto& gameObject : objectsGrass01)
	//{
	//	gameObject->SetName(L"Grass01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Grass_02
	//shared_ptr<MeshData> meshGrass02 = GET_SINGLE(Resources)->LoadFBX(L"Grass_02.bin");

	//vector<shared_ptr<GameObject>>	objectsGrass02 = meshGrass02->Instantiate();

	//for (auto& gameObject : objectsGrass02)
	//{
	//	gameObject->SetName(L"Grass02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Branch_01
	//shared_ptr<MeshData> meshBranch01 = GET_SINGLE(Resources)->LoadFBX(L"Branch_01.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsBranch01 = meshBranch01->Instantiate();
	//
	//for (auto& gameObject : objectsBranch01)
	//{
	//	gameObject->SetName(L"Branch01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Bush_01
	//shared_ptr<MeshData> meshBush01 = GET_SINGLE(Resources)->LoadFBX(L"Bush_01.bin");

	//vector<shared_ptr<GameObject>>	objectsBush01 = meshBush01->Instantiate();

	//for (auto& gameObject : objectsBush01)
	//{
	//	gameObject->SetName(L"Bush01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Bush_02
	//shared_ptr<MeshData> meshBush02 = GET_SINGLE(Resources)->LoadFBX(L"Bush_02.bin");

	//vector<shared_ptr<GameObject>>	objectsBush02 = meshBush02->Instantiate();

	//for (auto& gameObject : objectsBush02)
	//{
	//	gameObject->SetName(L"Bush02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Building_05
	//shared_ptr<MeshData> meshBuilding05 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_building_05.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsBuilding05 = meshBuilding05->Instantiate();
	//
	//for (auto& gameObject : objectsBuilding05)
	//{
	//	gameObject->SetName(L"Building05");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// chair_01a
	//shared_ptr<MeshData> meshChair01a = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_chair_01a.bin");

	//vector<shared_ptr<GameObject>>	objectsChair01a = meshChair01a->Instantiate();

	//for (auto& gameObject : objectsChair01a)
	//{
	//	gameObject->SetName(L"chair_01a");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// treePine01
	//shared_ptr<MeshData> meshTreePine01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_tree_pine_01.bin");

	//vector<shared_ptr<GameObject>>	objectsTreePine01 = meshTreePine01->Instantiate();

	//for (auto& gameObject : objectsTreePine01)
	//{
	//	gameObject->SetName(L"treePine01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_hanger_clothes_01
	//shared_ptr<MeshData> meshHanger01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_hanger_clothes_01.bin");

	//vector<shared_ptr<GameObject>>	objectsHanger01 = meshHanger01->Instantiate();

	//for (auto& gameObject : objectsHanger01)
	//{
	//	gameObject->SetName(L"Hanger01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_rock_01
	//shared_ptr<MeshData> meshNewRock01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_rock_01.bin");

	//vector<shared_ptr<GameObject>>	objectsNewRock01 = meshNewRock01->Instantiate();

	//for (auto& gameObject : objectsNewRock01)
	//{
	//	gameObject->SetName(L"NewRock01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_rock_02
	//shared_ptr<MeshData> meshNewRock2 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_rock_02.bin");

	//vector<shared_ptr<GameObject>>	objectsNewRock02 = meshNewRock2->Instantiate();

	//for (auto& gameObject : objectsNewRock02)
	//{
	//	gameObject->SetName(L"NewRock02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_rocks_tiny_01
	//shared_ptr<MeshData> meshTinyRock = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_rocks_tiny_01.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsTinyRock = meshTinyRock->Instantiate();
	//
	//for (auto& gameObject : objectsTinyRock)
	//{
	//	gameObject->SetName(L"TinyRock");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_table_01
	//shared_ptr<MeshData> meshTable01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_table_01.bin");

	//vector<shared_ptr<GameObject>>	objectsTable01 = meshTable01->Instantiate();

	//for (auto& gameObject : objectsTable01)
	//{
	//	gameObject->SetName(L"Table01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_tree_02
	//shared_ptr<MeshData> meshNewTree02 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_tree_02.bin");

	//vector<shared_ptr<GameObject>>	objectsNewTree02 = meshNewTree02->Instantiate();

	//for (auto& gameObject : objectsNewTree02)
	//{
	//	gameObject->SetName(L"NewTree02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_bush_01
	//shared_ptr<MeshData> meshNewBush01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_bush_01.bin");

	//vector<shared_ptr<GameObject>>	objectsNewBush01 = meshNewBush01->Instantiate();

	//for (auto& gameObject : objectsNewBush01)
	//{
	//	gameObject->SetName(L"NewBush01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_bush_02
	//shared_ptr<MeshData> meshNewBush02 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_bush_02.bin");

	//vector<shared_ptr<GameObject>>	objectsNewBush02 = meshNewBush02->Instantiate();

	//for (auto& gameObject : objectsNewBush02)
	//{
	//	gameObject->SetName(L"NewBush02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_bird_house_01
	//shared_ptr<MeshData> meshBirdHouse01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_bird_house_01.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsBirdHouse01 = meshBirdHouse01->Instantiate();
	//
	//for (auto& gameObject : objectsBirdHouse01)
	//{
	//	gameObject->SetName(L"BirdHouse01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}
	//
	//// rpgpp_lt_log_wood_01
	//shared_ptr<MeshData> meshLogWood01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_log_wood_01.bin");
	//
	//vector<shared_ptr<GameObject>>	objectsLogWood01 = meshLogWood01->Instantiate();
	//
	//for (auto& gameObject : objectsLogWood01)
	//{
	//	gameObject->SetName(L"LogWood01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_flower_01
	//shared_ptr<MeshData> meshFlower01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_flower_01.bin");

	//vector<shared_ptr<GameObject>>	objectsFlower01 = meshFlower01->Instantiate();

	//for (auto& gameObject : objectsFlower01)
	//{
	//	gameObject->SetName(L"Flower01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_flower_02
	//shared_ptr<MeshData> meshFlower02 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_flower_02.bin");

	//vector<shared_ptr<GameObject>>	objectsFlower02 = meshFlower02->Instantiate();

	//for (auto& gameObject : objectsFlower02)
	//{
	//	gameObject->SetName(L"Flower02");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(-10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// rpgpp_lt_flower_03
	//shared_ptr<MeshData> meshFlower03 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_flower_03.bin");

	//vector<shared_ptr<GameObject>>	objectsFlower03 = meshFlower03->Instantiate();

	//for (auto& gameObject : objectsFlower03)
	//{
	//	gameObject->SetName(L"Flower03");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(20.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}

	//// Fence
	//shared_ptr<MeshData> meshFlower01 = GET_SINGLE(Resources)->LoadFBX(L"rpgpp_lt_flower_01.bin");

	//vector<shared_ptr<GameObject>>	objectsFlower01 = meshFlower01->Instantiate();

	//for (auto& gameObject : objectsFlower01)
	//{
	//	gameObject->SetName(L"Flower01");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(10.f, -10.f, 40.f));
	//	gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
	//	scene->AddGameObject(gameObject);
	//}
}
