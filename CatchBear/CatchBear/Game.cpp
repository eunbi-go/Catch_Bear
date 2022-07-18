#include "stdafx.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"


void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	GET_SINGLE(SceneManager)->LoadScene(SCENE_ID::LOGIN);
}

void Game::Update()
{
	GEngine->Update();
	_isEnd = GET_SINGLE(SceneManager)->IsEnd();
}

void Game::LoginSceneUpdate()
{
	GEngine->LoginSceneUpdate();
}

void Game::LobbySceneUpdate()
{
	GEngine->LoginSceneUpdate();
}

void Game::setString(const char* strText)
{
	cout << strText << endl;
	wstring wstrText = s2ws(strText);
	GEngine->GetFontDevice()->UpdateFont(wstrText);
}

const wstring& Game::GetFontString()
{
	return GEngine->GetFontDevice()->GetFontString();
}

const bool Game::isIPAddrEnter()
{
	return GEngine->GetIsIPAddrEnter();
}

const bool Game::isAllPlayerReady()
{
	return GEngine->GetIsAllPlayerReady();
}
