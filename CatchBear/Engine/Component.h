#pragma once

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	// ....
	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	friend class GameObject;	// ���� ������Ʈ�� set �Լ��� ������ �� �ֵ��� �������
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;

	// ������Ʈ�� �ڱ� ������ �˾ƾ� �ϰ�, ���� ������Ʈ�� ���� ����ִ� ������Ʈ�� �˾ƾ� ��
	// ���� ������ �� shared_ptr�� ����� �Ǹ� ��ȯ ������ ��������� �ż�
	// ���� ������Ʈ�� ������Ʈ�� ����Ű�� ������Ʈ�� ���� ������Ʈ�� ����Ű�� ������
	// ���۷��� ī��Ʈ�� �پ���� �ʴ� ������ �߻��ϱ� ������ weak_ptr�� �������
	weak_ptr<GameObject> _gameObject;	

};
