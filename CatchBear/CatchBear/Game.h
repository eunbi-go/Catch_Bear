#pragma once
class SceneManager;
class Game
{
public:
	void Init(const WindowInfo& info);
	void Update();
	void LoginSceneUpdate();
	void LobbySceneUpdate();

	void setString(const char* strText);
	void UpdateFont(const char* strText);
	const wstring& GetFontString();
	const bool isIPAddrEnter();
	const bool isAllPlayerReady();

	SCENE_ID GetCurSceneID();

public:
	bool	_isEnd = false;
	bool	_isFirstEnter = true;
};

