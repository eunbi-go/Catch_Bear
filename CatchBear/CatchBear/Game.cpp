#include "stdafx.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"


void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	GET_SINGLE(SceneManager)->LoadScene(L"LoginScene");
}

void Game::Update()
{
	GEngine->Update();
	_isEnd = GET_SINGLE(SceneManager)->IsEnd();
}

void Game::setString(const char* strText)
{
	wstring wstrText = s2ws(strText);
	GEngine->GetFontDevice()->UpdateFont(wstrText);
}

const wstring& Game::GetFontString()
{
	return GEngine->GetFontDevice()->GetFontString();
}
