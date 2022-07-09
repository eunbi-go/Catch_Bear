#include "pch.h"
#include "LobbyScene.h"
#include "Engine.h"
#include "SoundManager.h"
#include "GameObject.h"
#include "Input.h"
#include "SceneManager.h"

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::Awake()
{
	//GET_SINGLE(SoundManager)->StopAll();
	//GET_SINGLE(SoundManager)->PlayBGM(L"Stage.mp3");
}

void LobbyScene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void LobbyScene::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::ENTER)) {
		GET_SINGLE(SceneManager)->LoadScene(SCENE_ID::STAGE);
		GEngine->_isIPAddrEnter = true;
	}
}

void LobbyScene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void LobbyScene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}
