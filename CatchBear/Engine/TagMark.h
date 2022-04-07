#pragma once
#include "MonoBehaviour.h"
class GameObject;

class TagMark : public MonoBehaviour, public enable_shared_from_this<TagMark>
{
public:
	TagMark();
	virtual ~TagMark();

	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	shared_ptr<GameObject>	_tagMark = make_shared<GameObject>();
	float _fRotationSpeed = 5.f;
};

