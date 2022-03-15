#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;
class ParticleSystem;
class Terrain;
class BaseCollider;

// Component 무리들을 관리하는 조립체
class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

public:
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	vector<shared_ptr<MonoBehaviour>>& GetScripts() { return _scripts; }
	shared_ptr<MonoBehaviour>& GetScript(int index);

public:
	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();
	shared_ptr<ParticleSystem> GetParticleSystem();
	shared_ptr<Terrain> GetTerrain();
	shared_ptr<BaseCollider> GetCollider();

public:
	void AddComponent(shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

	void SetStatic(bool flag) { _static = flag; }
	bool IsStatic() { return _static; }

private:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;	// 일반적인 컴포넌트(고정)
	vector<shared_ptr<MonoBehaviour>> _scripts;		// 유저가 스크립트를 이용해 만들어주는 컴포넌트

	bool	_checkFrustum = true;
	uint8	_layerIndex = 0;
	bool	_static = true;
};

