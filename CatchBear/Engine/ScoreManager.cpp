#include "pch.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

void ScoreManager::Update()
{
	AddScore();
}

void ScoreManager::AddScore()
{
	// 1초에 1점씩 올라감
	// 보물을 찾으면 30점 얻음
	// 술래는 보물 찾아도 점수 얻지 않음

	auto& scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_myPlayer = scene->GetGameObject(L"Player");

	//if (_myPlayer->GetIsTagger()) return;		// 내가 테스트할땐 플레이어 한명만 만들어서(걔가 술래됨) 꺼둠, 원래 켜야됨!

	_scoreTime += DELTA_TIME;

	if (_scoreTime >= 1.f)
	{
		static_pointer_cast<Player>(_myPlayer->GetScript(0))->AddPlayerScore(1);
		printf("%d\n", static_pointer_cast<Player>(_myPlayer->GetScript(0))->GetPlayerScore());
		_scoreTime = 0.f;
	}
}

void ScoreManager::Rank()
{
}
