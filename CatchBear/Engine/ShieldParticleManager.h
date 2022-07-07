#pragma once

class MeshData;

class ShieldParticleManager
{
	DECLARE_SINGLE(ShieldParticleManager);

public:
	void Init();
	void Update();
	void LateUpdate();

public:
	void UpdatePlayerPos(Vec3 playerPos) { _playerPos = playerPos; }

public:
	void SetShieldParticleOn() { _shieldParticleOn = true; }
	void SetShieldParticleOff() { _shieldParticleOn = false; }

private:
	shared_ptr<MeshData> _shieldMesh;

	Vec3	_playerPos;
	bool	_shieldParticleOn = false;
	float	_shieldTime = 5.f;
	float	_delayTime = 0.5f;
	float	_curTime = 0.f;
};

