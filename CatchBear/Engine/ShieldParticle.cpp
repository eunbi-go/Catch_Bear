#include "pch.h"
#include "ShieldParticle.h"
#include "Timer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "ShieldParticleManager.h"

ShieldParticle::ShieldParticle()
{
	random_device rd;
	uniform_real_distribution<float> speed(0.5, 3);
	_speed = speed(rd);
}

ShieldParticle::~ShieldParticle()
{
}

void ShieldParticle::LateUpdate()
{
	_curTime += DELTA_TIME;
	
	if (_curTime <= _lifeTime)
	{
		_playerPos = GET_SINGLE(ShieldParticleManager)->GetPlayerPos();

		_startPos.y += _speed * DELTA_TIME;
		_startPos.x = _playerPos.x + _disX;
		_startPos.z = _playerPos.z + _disZ;

		GetTransform()->SetLocalPosition(_startPos);
	}

	else
	{
		_isDead = true;
		//GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(GetGameObject());
	}
}
