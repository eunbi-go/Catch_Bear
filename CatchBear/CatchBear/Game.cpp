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
	//GEngine->LobbySceneUpdate();
}

void Game::setString(const char* strText)
{
	wstring wstrText = s2ws(strText);
	GEngine->GetFontDevice()->UpdateFont(wstrText);
}

void Game::UpdateFont(const char* strText)
{
	wstring wstrText = s2ws(strText);
	GEngine->GetFontDevice()->PushFont(wstrText);
}

void Game::SendChatPacket(const char* strText)
{
	wstring wstrText = s2ws(strText);
	GEngine->GetFontDevice()->SendChatPacket(wstrText);
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

SCENE_ID Game::GetCurSceneID()
{
	return GET_SINGLE(SceneManager)->getSceneID();
}
