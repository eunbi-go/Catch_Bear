#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"
#include "AnimationController.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	if (GetAnimationController())
	{
		GetAnimationController()->PushData();
	}
	_material->PushGraphicsData();
	_mesh->Render(1, 0);
}

void MeshRenderer::Render(shared_ptr<InstancingBuffer>& buffer)
{
	buffer->PushData();

	// Animation
	if (GetAnimationController())
	{
		GetAnimationController()->PushData();
	}

	_material->PushGraphicsData();
	_mesh->Render(buffer, 0);
}

void MeshRenderer::RenderShadow()
{
	GetTransform()->PushData();
	GET_SINGLE(Resources)->Get<Material>(L"Shadow")->PushGraphicsData();
	_mesh->Render();
}

uint64 MeshRenderer::GetInstanceID()
{
	// 각 mesh, material을 만들때마다 ID를 부여, 이 ID를 통해 새로운 ID를 조합해서 만들어줌

	if (_mesh == nullptr || _material == nullptr)
		return 0;

	// uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}
