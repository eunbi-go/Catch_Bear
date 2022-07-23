#pragma once


class Timer
{
	DECLARE_SINGLE(Timer);

public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }
	float GetAllTime() { return _AllTime; }
	void	AddTime() { _AllTime -= 30.f; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;

	// 이전 프레임에서 현재 프레임까지 경과된 시간
	float	_deltaTime = 0.f;

private:
	// Frame
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;
	uint32	_fps = 0;
	float	_AllTime = 0.f;
};

