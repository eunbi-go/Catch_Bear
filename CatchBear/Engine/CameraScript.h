#pragma once
#include "MonoBehaviour.h"

// 플레이어에게 고정된 카메라
class CameraScript : public MonoBehaviour
{
public:
	CameraScript();
	virtual ~CameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
};

