#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

// Component 무리들을 관리하는 조립체
class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	ㄴ\
	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;	// 일반적인 컴포넌트(고정)
	vector<shared_ptr<MonoBehaviour>> _scripts;		// 유저가 스크립트를 이용해 만들어주는 컴포넌트
};

