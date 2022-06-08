#pragma once

class Game
{
public:
	void Init(const WindowInfo& info);
	void Update();

	void setString(const char* strText);
	const wstring& GetFontString();

public:
	bool	_isEnd = false;
};

