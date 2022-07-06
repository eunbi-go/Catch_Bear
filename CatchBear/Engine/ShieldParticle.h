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
	void ShieldParticleOn();

public:
	void SetShieldParticleOn(Vec3 playerPos) { isOn = true; _playerPos = playerPos; }
	void SetShieldParticleOff() { isOn = false; }

private:
	//shared_ptr<GameObject> _player = make_shared<GameObject>();
	Vec3 _playerPos;
	bool isOn = false;
};

