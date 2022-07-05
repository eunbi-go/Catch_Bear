#pragma once
#include "MonoBehaviour.h"

class Button : public MonoBehaviour
{
public:
	Button();
	virtual ~Button();

	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	shared_ptr<GameObject>		_button = make_shared<GameObject>();
};

