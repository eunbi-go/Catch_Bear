#pragma once
#include "MonoBehaviour.h"

class Texture;

class Button : public MonoBehaviour
{
public:
	Button();
	virtual ~Button();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;


public:
	void SetCollRect(RECT rect) { _rect = rect; }
	void SetCollTexture(shared_ptr<Texture> texture) { _collTexture = texture; }
	void SetOriginalTexture(shared_ptr<Texture> texture) { _originalTexture = texture; }



private:
	shared_ptr<GameObject>		_button = make_shared<GameObject>();

	RECT	_rect;
	POINT	_mousePos;

	shared_ptr<Texture>	_collTexture;
	shared_ptr<Texture>	_originalTexture;
};

