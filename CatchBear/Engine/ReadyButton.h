#pragma once
#include "Button.h"

class ReadyButton : public Button
{
public:
	ReadyButton();
	virtual ~ReadyButton();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	bool	_isReady = false;
};

