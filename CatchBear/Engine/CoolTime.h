#pragma once
#include "MonoBehaviour.h"

class CoolTime : public MonoBehaviour
{
public:
	CoolTime();
	virtual ~CoolTime();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	void SetInitData(const Vec3& pos, const Vec3& scale, int nSlot);

private:
	shared_ptr<GameObject>		_coolTime;
	float	_fCoolTime = 5.f;
	bool	_isCoolTime = false;

	Vec3	_localPos;
	Vec3	_localScale;
	int		_nSlot = -1;
};