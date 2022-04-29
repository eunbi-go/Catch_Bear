#pragma once

class Game
{
public:
	void Init(const WindowInfo& info);
	void Update();

public:
	bool	_isEnd = false;
};

