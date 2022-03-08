#pragma once
#include "MonoBehaviour.h"

class Player : public MonoBehaviour
{
public:
	Player();
	virtual ~Player();

	virtual void LateUpdate() override;

private:
	void KeyCheck();


private:
	float	_speed = 5.f;
};

