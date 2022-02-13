#pragma once
#include "Object.h"

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	PARTICLE_SYSTEM,
	TERRAIN,
	COLLIDER,
	// ....
	MONO_BEHAVIOUR,		// 마지막 순서여야함
	END,
};

enum
{
	// MONO_BEHAVIOUR를 제외한 애들은 자기만의 슬롯 번호를 가지고 있음,
	// 번호에 맞춰서 게임 오브젝트에서 하나만 가질 수 있음
	// MONO_BEHAVIOUR는 여러개 만들 수 있기 때문에 별도로 생각함
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;	// 게임 오브젝트만 set 함수를 실행할 수 있도록 허용해줌
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;

	// 컴포넌트도 자기 주인을 알아야 하고, 게임 오브젝트도 내가 들고있는 컴포넌트로 알아야 함
	// 만약 양쪽을 다 shared_ptr로 만들게 되면 순환 구조가 만들어지게 돼서
	// 게임 오브젝트는 컴포넌트를 가리키고 컴포넌트는 게임 오브젝트를 가리키기 때문에
	// 래퍼런스 카운트가 줄어들지 않는 문제가 발생하기 때문에 weak_ptr로 만들어줌
	weak_ptr<GameObject> _gameObject;	

};

