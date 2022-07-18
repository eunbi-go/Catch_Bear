#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
public:
	LobbyScene();
	virtual ~LobbyScene();

public:
	// Scene을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

private:
	void CheckIsPlayerEnter();
};

