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
	float	_rotSpeed = 2.f;

	shared_ptr<GameObject>		_player = make_shared<GameObject>();
	shared_ptr<GameObject>		_camera = make_shared<GameObject>();
	shared_ptr<class CameraScript>	_cameraScript = make_shared<CameraScript>();
};

