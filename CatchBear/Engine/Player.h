#pragma once
#include "MonoBehaviour.h"

class Player : public MonoBehaviour, public enable_shared_from_this<Player>
{
public:
	//enum STATE
	//{
	//	IDLE, WALK, DASH, JUMP, ATTACK, END
	//};

public:
	Player();
	virtual ~Player();

	virtual void LateUpdate() override;

private:
	void KeyCheck();

private:
	float	_speed = 15.f;
	float	_rotSpeed = 2.f;

	shared_ptr<GameObject>		_player = make_shared<GameObject>();
	shared_ptr<GameObject>		_camera = make_shared<GameObject>();
	shared_ptr<class CameraScript>	_cameraScript = make_shared<CameraScript>();

private:
	STATE	_curState = STATE::END;
	STATE	_preState = STATE::END;

public:
	class PlayerState* _state;
};

