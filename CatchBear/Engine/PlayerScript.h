#pragma once

#include "MonoBehaviour.h"

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void LateUpdate() override;
};

