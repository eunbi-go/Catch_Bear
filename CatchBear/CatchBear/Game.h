#pragma once

class Game
{
public:
	void Init(const WindowInfo& info);
	void Update();

	void setString(const char* strText);

public:
	bool	_isEnd = false;
};

