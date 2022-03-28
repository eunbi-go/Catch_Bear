#include "pch.h"
#include "CameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"

CameraScript::CameraScript()
{
	m_xmf3Offset = Vec3(0.0f, 20.f, -50.f);
}

CameraScript::~CameraScript()
{
}

void CameraScript::LateUpdate()
{
}

void CameraScript::Revolve(float rotSize, Vec3 playerPos)
{
	//if (INPUT->GetButton(KEY_TYPE::W))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x += DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::S))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.x -= DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::A))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y += DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}

	//if (INPUT->GetButton(KEY_TYPE::D))
	//{
	//	Vec3 rotation = GetTransform()->GetLocalRotation();
	//	rotation.y -= DELTA_TIME * 0.5f;
	//	GetTransform()->SetLocalRotation(rotation);
	//}


	///////

	//Vec3 pos = GetTransform()->GetLocalPosition();

	//if (INPUT->GetButton(KEY_TYPE::UP))
	//	pos += GetTransform()->GetUp() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::DOWN))
	//	pos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::LEFT))
	//	pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	//if (INPUT->GetButton(KEY_TYPE::RIGHT))
	//	pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	//GetTransform()->SetLocalPosition(pos);

	if (_player)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationY(rotSize);
		m_xmf3Offset = Vector3::TransformNormal(m_xmf3Offset, xmmtxRotate);

		XMFLOAT4X4 xm4x4Rotate = Matrix4x4::Identity();
		XMFLOAT3 xmf3Offset = Vector3::TransformCoord1(m_xmf3Offset, xm4x4Rotate);
		XMFLOAT3 xmOldPosition = GetTransform()->GetLocalPosition();
		XMFLOAT3 xmf3Position = Vector3::Add(playerPos, xmf3Offset);

		XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, xmOldPosition);
		float fLength = Vector3::Length(xmf3Direction);
		xmf3Direction = Vector3::Normalize(xmf3Direction);

		xmOldPosition = Vector3::Add(xmOldPosition, xmf3Direction, fLength);
		GetTransform()->SetLocalPosition(xmOldPosition);
		Vec3 dir = Vector3::Subtract(playerPos, xmOldPosition);
		dir.Normalize();
		GetTransform()->LookAt(dir);
	}
}