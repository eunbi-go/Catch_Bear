#include "pch.h"
#include "CoolTime.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Component.h"
#include "Resources.h"
#include "Timer.h"
#include "Transform.h"

CoolTime::CoolTime()
{
}

CoolTime::~CoolTime()
{
}

void CoolTime::Update()
{
	if (GetGameObject()->_isRender)
	{
		if (_fCoolTime >= 0.6f)
		{
			_fCoolTime -= DELTA_TIME;

			float fScale = 5.f / _fCoolTime;
			Vec3	localScale = GetGameObject()->GetTransform()->GetLocalScale();
			GetGameObject()->GetTransform()->SetLocalScale(Vec3(localScale.x, localScale.y - fScale, localScale.z));

			float fPos = fScale / 2.f;
			Vec3	localPos = GetGameObject()->GetTransform()->GetLocalPosition();
			GetGameObject()->GetTransform()->SetLocalPosition(Vec3(localPos.x, localPos.y - fPos, localPos.z));
		}
		else
		{
			_fCoolTime = 0;
			GetGameObject()->_isRender = false;
			GetGameObject()->GetTransform()->SetLocalScale(_localScale);
			GetGameObject()->GetTransform()->SetLocalPosition(_localPos);
		}
	}
}

void CoolTime::LateUpdate()
{
}

void CoolTime::SetInitData(const Vec3& pos, const Vec3& scale)
{
	GetGameObject()->_isRender = true;
	_localPos = pos;
	_localScale = scale;
}
