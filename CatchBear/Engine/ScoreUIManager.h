#pragma once
#include "Item.h"
class GameObject;

class ScoreUIManager
{
	DECLARE_SINGLE(ScoreUIManager);

public:
	

private:
	shared_ptr<GameObject> _scoreUI1;
	shared_ptr<GameObject> _scoreUI2;
	shared_ptr<GameObject> _scoreUI3;
};