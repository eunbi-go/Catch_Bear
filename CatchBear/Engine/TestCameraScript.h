#pragma once
#include "MonoBehaviour.h"

// 키보드 입력에 따라 카메라 위치를 바꿔줌
class TestCameraScript : public MonoBehaviour
{
public:
	TestCameraScript();
	virtual ~TestCameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
};

