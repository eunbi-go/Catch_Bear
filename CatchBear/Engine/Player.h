#pragma once
#include "GameObject.h"

class Player : public GameObject
{
	Player();
	virtual ~Player();

	virtual void LateUpdate() override;

};

