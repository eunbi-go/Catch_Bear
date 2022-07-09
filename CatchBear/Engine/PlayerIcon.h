#pragma once
#include "Button.h"

class PlayerIcon : public Button
{
public:
	PlayerIcon();
	virtual ~PlayerIcon();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	PLAYER_TYPE CheckPlayerType(const wstring& texKey);
};

