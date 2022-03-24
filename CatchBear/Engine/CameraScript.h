#pragma once
#include "MonoBehaviour.h"

// 플레이어에게 고정된 카메라
class CameraScript : public MonoBehaviour
{
public:
	CameraScript();
	virtual ~CameraScript();

	virtual void LateUpdate() override;

public:
	void Revolve(float rotSize, Vec3 playerPos);

private:
	float		_speed = 5.f;
	Vec3		_offset;

	///////
	shared_ptr<GameObject>	_player = make_shared<GameObject>();
	XMFLOAT3 m_xmf3Offset;		// 플레이어와 카메라의 오프셋을 나타내는 벡터. 주로 3인칭 카메라에서 사용됨
	///////
};

