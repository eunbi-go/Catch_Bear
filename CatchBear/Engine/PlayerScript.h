#pragma once

#include "MonoBehaviour.h"

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

private:
	void KeyCheck();


private:
	float	_speed = 5.f;
};

