#pragma once

#include "MonoBehaviour.h"
class GameObject;

class ShieldParticle : public MonoBehaviour
{
public:
	ShieldParticle();
	virtual ~ShieldParticle();

public:
	virtual void LateUpdate() override;

private:
	Vec3	_startPos;
	float	_speed;
};

