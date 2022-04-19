#pragma once
#include "Item.h"
class GameObject;

class ScoreUIManager
{
	DECLARE_SINGLE(ScoreUIManager);

public:
	// nPlayer번째의 점수인 nScore로 텍스처를 설정한다
	void SetPlayerScoreTexture(int nPlayer, int nScore);

private:
	shared_ptr<GameObject> _scoreUI1;
	shared_ptr<GameObject> _scoreUI2;
	shared_ptr<GameObject> _scoreUI3;
};