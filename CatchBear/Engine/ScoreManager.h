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

private:
	void AddScore();

private:
	float	_scoreTime = 0.f;

	shared_ptr<GameObject>	_myPlayer;
	array<int, 3> _playerScore;	// 0번 인덱스는 무조건 내 점수
};

