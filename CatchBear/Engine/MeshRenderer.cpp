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
	// �ѹ��� �׷���� �ϴ�, �ν��Ͻ��� �� �ֵ��� params.fx�� TRANSFORM_PARAMS : register(b1)�� ����ϴ°� �ƴ϶�
	// �츮�� ���� �־��� deferred.fx�� VS_IN �κп��� �޾��ְ� �ȴ�.

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
	// shadow�� �ν��Ͻ� ������� �ʰ� �Ϲ�����(�ϳ��� �׸���) ������� �׸�
	
	// �������� ��� ��ü���� �ڽ��� material�� �̿��ؼ� �о�ְ� �־��µ�
	// shadow�� �׸��� �̸� ����� ���� shadow material�� �̿��ؼ� �װͿ� �ش��ϴ� �������� push�ؼ� render�ϰ� ����

	GetTransform()->PushData();
	GET_SINGLE(Resources)->Get<Material>(L"Shadow")->PushGraphicsData();
	_mesh->Render();
}

uint64 MeshRenderer::GetInstanceID()
{
	// �� mesh, material�� ���鶧���� ID�� �ο�, �� ID�� ���� ���ο� ID�� �����ؼ� �������

	if (_mesh == nullptr || _material == nullptr)
		return 0;

	// uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}
