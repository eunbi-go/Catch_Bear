#include "pch.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ScoreUIManager.h"

void ScoreManager::Update()
{
	if (GET_SINGLE(SceneManager)->GetActiveScene()->_isStart)
		AddScore();
}

const vector<shared_ptr<GameObject>>& ScoreManager::GetVecRankedPlayers()
{
	if (_bRanked)
		return _vecRankedPlayers;
}

void ScoreManager::AddScore()
{
	// 1초에 1점씩 올라감
	// 보물을 찾으면 30점 얻음
	// 술래는 보물 찾아도 점수 얻지 않음

	auto& scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_myPlayer = scene->GetGameObject(L"Player1");

	//if (_myPlayer->GetIsTagger()) return;		// 내가 테스트할땐 플레이어 한명만 만들어서(걔가 술래됨) 꺼둠, 원래 켜야됨!

	_scoreTime += DELTA_TIME;

	if (_scoreTime >= 1.f)
	{
		static_pointer_cast<Player>(_myPlayer->GetScript(0))->AddPlayerScore(1);
		GET_SINGLE(ScoreUIManager)->SetPlayerScoreTexture(0, static_pointer_cast<Player>(_myPlayer->GetScript(0))->GetPlayerScore());
		//printf("Score: %d\n", static_pointer_cast<Player>(_myPlayer->GetScript(0))->GetPlayerScore());
		_scoreTime = 0.f;
	}
}

void ScoreManager::Rank()
{
	auto& scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_vecRankedPlayers = scene->GetVecPlayers();

	sort(_vecRankedPlayers.begin(), _vecRankedPlayers.end(),
		[](shared_ptr<GameObject>& p1, shared_ptr<GameObject>& p2)
		{
			return static_pointer_cast<Player>(p1->GetScript(0))->GetPlayerScore()
				< static_pointer_cast<Player>(p2->GetScript(0))->GetPlayerScore();
		});

	_bRanked = true;
}
