#pragma once
#include "GameObject.h"

class TimerUI : public GameObject
{
public:
	TimerUI();
	virtual ~TimerUI();

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();

private:
};

