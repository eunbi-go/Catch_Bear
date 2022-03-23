#pragma once
#include "Component.h"


class Transform : public Component, public enable_shared_from_this<Transform>
{
public:
	Transform();
	virtual ~Transform();

	// 엔진에서 모든 작업이 끝난 후 최종적으로 행렬 연산과 관련된 작업을 해줌
	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent 기준
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	// TEMP - 나중에 바운딩 박스같은거 만들때 다시 생각함, scale 이용해서 물체의 구를 임시로 판단
	// _localScale에서 가장 큰 성분을 boundingSphere radius로 인정
	float GetBoundingSphereRadius() { return max(max(_localScale.x, _localScale.y), _localScale.z); }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; UpdateTransform(NULL);
	}
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; UpdateTransform(NULL);
	}
	void SetLocalScale(const Vec3& scale) { _localScale = scale; UpdateTransform(NULL);
	}
	//void SetLocalToParent(const Matrix& toParent) { _matToParent = toParent; }

	void LookAt(const Vec3& dir);

	static bool CloseEnough(const float& a, const float& b, const float& epsilon = std::numeric_limits<float>::epsilon());
	static Vec3 DecomposeRotationMatrix(const Matrix& rotation);

public:
	shared_ptr<Transform>	FindTransform(wstring name);

	void UpdateTransform(Matrix* matParent);

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	void SetChild(shared_ptr<Transform> child);
	void SetName(wstring name) { _name = name; }

public:
	shared_ptr<Transform> GetParent() { return _parent; }

private:
	// TODO: World 위치 관련
	Vec3	_localPosition = {};
	Vec3	_localRotation = {};
	Vec3	_localScale = { 1.f, 1.f, 1.f };

public:

	Matrix	_matLocal = {};
	Matrix	_matWorld = {};
	// 계층구조에서 부모에 상대적인 행렬
	Matrix	_matToParent = {};

	shared_ptr<Transform>		_parent;	// shared_ptr로 만들면 순환관계가 생김
	shared_ptr<Transform>		_child;
	shared_ptr<Transform>		_sibling;

	wstring	_name;
};

