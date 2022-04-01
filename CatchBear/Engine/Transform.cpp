#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{
}

void Transform::FinalUpdate()
{
	// 엔진에서 모든 작업이 끝나고 최종적으로 행렬 관련 연산을 해주기 위한 단계
	// Awake ~ LateUpdate까지 모두 완료한 후에
	// 월드 행렬을 만들어줌
	// SRT 계산
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	// SRT 구한 것을 로컬에 넣어줌
	// 부모가 없는 상태라면 SRT 자체가 월드행렬을 얘기하는 것
	_matLocal = matScale * matRotation * matTranslation;	// 부모의 local space로 가기 위한 행렬
	_matWorld = _matLocal;									// world space로 가기 위한 행렬

	//shared_ptr<Transform> parent = GetParent().lock();
	//shared_ptr<Transform> parent = GetParent();
	//if (parent != nullptr)
	//{
	//	_matWorld *= parent->GetLocalToWorldMatrix();
	//}
}

void Transform::PushData()
{
	// 데이터를 최종적으로 GPU에 건네줌
	// WVP, W는 FinalUpdate에서 만듦
	// View, Projection - Camera에서 만듦
	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	transformParams.matViewInv = Camera::S_MatView.Invert();

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushGraphicsData(&transformParams, sizeof(transformParams));
}

void Transform::LookAt(const Vec3& dir)
{
	Vec3 front = dir;
	front.Normalize();

	Vec3 right = Vec3::Up.Cross(dir);
	if (right == Vec3::Zero)
		right = Vec3::Forward.Cross(dir);

	right.Normalize();

	Vec3 up = front.Cross(right);
	up.Normalize();

	Matrix matrix = XMMatrixIdentity();
	matrix.Right(right);
	matrix.Up(up);
	matrix.Backward(front);

	_localRotation = DecomposeRotationMatrix(matrix);
}

bool Transform::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

Vec3 Transform::DecomposeRotationMatrix(const Matrix& rotation)
{
	Vec4 v[4];
	XMStoreFloat4(&v[0], rotation.Right());
	XMStoreFloat4(&v[1], rotation.Up());
	XMStoreFloat4(&v[2], rotation.Backward());
	XMStoreFloat4(&v[3], rotation.Translation());

	Vec3 ret;
	if (CloseEnough(v[0].z, -1.0f))
	{
		float x = 0;
		float y = XM_PI / 2;
		float z = x + atan2(v[1].x, v[2].x);
		ret = Vec3{ x, y, z };
	}
	else if (CloseEnough(v[0].z, 1.0f))
	{
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(-v[1].x, -v[2].x);
		ret = Vec3{ x, y, z };
	}
	else
	{
		float y1 = -asin(v[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(v[1].z / cos(y1), v[2].z / cos(y1));
		float x2 = atan2f(v[1].z / cos(y2), v[2].z / cos(y2));

		float z1 = atan2f(v[0].y / cos(y1), v[0].x / cos(y1));
		float z2 = atan2f(v[0].y / cos(y2), v[0].x / cos(y2));

		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			ret = Vec3{ x1, y1, z1 };
		}
		else
		{
			ret = Vec3{ x2, y2, z2 };
		}
	}

	return ret;
}

shared_ptr<Transform> Transform::FindTransform(wstring name)
{
	shared_ptr<Transform>	frame = make_shared<Transform>();
	if (_name == name) return shared_from_this();
	
	if (_sibling)
	{
		if (frame = _sibling->FindTransform(name))
			return frame;
	}
	if (_child)
	{
		if (frame = _child->FindTransform(name))
			return frame;
	}
	
	return NULL;
}

void Transform::UpdateTransform(Matrix* matParent)
{
	_matWorld = (matParent) ? Multiply(_matToParent, *matParent) : _matToParent;

	if (_sibling)	_sibling->UpdateTransform(matParent);
	if (_child)		_child->UpdateTransform(&_matWorld);
}

void Transform::PreRender()
{
	_matToParent._11 = GetRight().x; _matToParent._12 = GetRight().y; _matToParent._13 = GetRight().z;
	_matToParent._21 = GetUp().x; _matToParent._22 = GetUp().y; _matToParent._23 = GetUp().z;
	_matToParent._31 = GetLook().x; _matToParent._32 = GetLook().y; _matToParent._33 = GetLook().z;
	_matToParent._41 = GetLocalPosition().x; _matToParent._42 = GetLocalPosition().y; _matToParent._43 = GetLocalPosition().z;

	_matToParent = Multiply(XMMatrixScaling(_localScale.x, _localScale.y, _localScale.z), _matToParent);

}


void Transform::SetChild(shared_ptr<Transform> child)
{
	if (child)
		child->_parent = shared_from_this();
	if (_child)
	{
		if (child)	child->_sibling = _child->_sibling;
		_child->_sibling = child;
	}
	else _child = child;
}

void Transform::SettoParentMat(wstring name, Matrix toParent)
{
	if (name == _name)
	{
		_matToParent = toParent;
		return;
	}

	else
	{
		if (_child)	_child->SettoParentMat(name, toParent);
		if (_sibling)	_sibling->SettoParentMat(name, toParent);
	}

}

