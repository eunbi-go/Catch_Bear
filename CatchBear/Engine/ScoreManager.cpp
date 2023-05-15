#include "pch.h"
#include "ScoreManager.h"
#include "Timer.h"
#include "Player.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ScoreUIManager.h"
#include "ServerSession.h"

void ScoreManager::Update()
{
	if (GET_SINGLE(SceneManager)->GetActiveScene()->_isStart)
		AddScore();
}

const vector<shared_ptr<GameObject>>& ScoreManager::GetVecRankedPlayers()
{
	//if (_bRanked)
		return _vecRankedPlayers;

}

void ScoreManager::InitScore()
{
	auto& scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_vecRankedPlayers = scene->GetVecPlayers();

	for (auto player : _vecRankedPlayers)
	{
		static_pointer_cast<Player>(player->GetScript(0))->SetPlayerScore(0);
	}
}

void ScoreManager::AddScore()
{
	// 1�ʿ� 1���� �ö�
	// ������ ã���� 30�� ����
	// ������ ���� ã�Ƶ� ���� ���� ����

	auto& scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// ��� �÷��̾ ������ �ö󰡰� ������
	_myPlayer = scene->GetPlayer(mysession->GetPlayerID());	

	_scoreTime += DELTA_TIME;

	if (_scoreTime >= 1.f && !scene->_isFinish)
	{
		// player1
		shared_ptr<GameObject>	player1 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(L"Player1");
		GET_SINGLE(ScoreUIManager)->SetPlayerScoreTexture(0, static_pointer_cast<Player>(player1->GetScript(0))->GetPlayerScore());

		// player2
		shared_ptr<GameObject>	player2 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(L"Player2");
		GET_SINGLE(ScoreUIManager)->SetPlayerScoreTexture(1, static_pointer_cast<Player>(player2->GetScript(0))->GetPlayerScore());

		//player3
		shared_ptr<GameObject>	player3 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(L"Player3");
		GET_SINGLE(ScoreUIManager)->SetPlayerScoreTexture(2, static_pointer_cast<Player>(player3->GetScript(0))->GetPlayerScore());

		// ���� �׽�Ʈ�Ҷ� �÷��̾� �Ѹ� ����(�°� ������) ����, ���� �Ѿߵ�!

		if (!_myPlayer->GetIsTagger())
			static_pointer_cast<Player>(_myPlayer->GetScript(0))->AddPlayerScore(1);
		else
		{
			if (!_bTaggerReset)
			{
				//static_pointer_cast<Player>(_myPlayer->GetScript(0))->SetPlayerScore(0);
				_bTaggerReset = true;
			}
		}
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
				> static_pointer_cast<Player>(p2->GetScript(0))->GetPlayerScore();
		});

	_bRanked = true;
}
