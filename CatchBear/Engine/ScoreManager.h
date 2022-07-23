#pragma once

// 1초에 1점씩 올라감
// 보물을 찾으면 30점 얻음 (임시값)

class GameObject;

class ScoreManager
{
	DECLARE_SINGLE(ScoreManager);


public:
	void Update();

public:
	void Rank();

public:
	//const int GetScore() { return _score; }
	//void SetScore(int score) { _score += score; }
	void SetMyPlayer(shared_ptr<GameObject> myPlayer) { _myPlayer = myPlayer; }
	const vector<shared_ptr<GameObject>>& GetVecRankedPlayers();

private:
	void AddScore();

private:
	float	_scoreTime = 0.f;
	bool	_bRanked = false;
	bool	_bTaggerReset = false;

	shared_ptr<GameObject>	_myPlayer;

	// 게임 끝나고 신에서 모아둔 플레이어를 가져와서 순위 매김
	vector<shared_ptr<GameObject>>	_vecRankedPlayers;

	array<int, 3> _playerScore;	// 0번 인덱스는 무조건 내 점수
};

