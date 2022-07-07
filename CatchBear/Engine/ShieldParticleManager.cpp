#include "pch.h"
#include "ShieldParticleManager.h"
#include "ShieldParticle.h"
#include "Timer.h"
#include "GameObject.h"
#include "MeshData.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SceneManager.h"

void ShieldParticleManager::Init()
{
	_shieldMesh = GET_SINGLE(Resources)->LoadFBX(L"present1.bin");
}

void ShieldParticleManager::Update()
{
	if (_shieldParticleOn)
	{
		for (int i = 0; i < 3; ++i)
		{
			vector<shared_ptr<GameObject>> particle = _shieldMesh->Instantiate();

			for (auto& gameObject : particle)
			{
				gameObject->SetName(L"ShieldParticle");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(_playerPos);
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.5f, 0.5f, 0.5f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				gameObject->AddComponent(make_shared<ShieldParticle>());
				GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(gameObject);
			}
		}
	}
}

void ShieldParticleManager::LateUpdate()
{

}
