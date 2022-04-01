#include "pch.h"
#include "ScoreManager.h"
#include "Timer.h"

void ScoreManager::Update()
{
	AddScore();
}

void ScoreManager::AddScore()
{
	// 1초에 1점씩 올라감
	// 보물을 찾으면 30점 얻음 (임시값)

	_scoreTime += DELTA_TIME;

	if (_scoreTime >= 1.f)
	{
		_score += 1;
		_scoreTime = 0.f;
	}
}
