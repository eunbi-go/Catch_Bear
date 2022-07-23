#include "pch.h"
#include "Button.h"
#include "Engine.h"
#include "GameObject.h"
#include "Input.h"
#include "MeshRenderer.h"
#include "Material.h"

Button::Button()
{
}

Button::~Button()
{
}

void Button::Update()
{


}

void Button::LateUpdate()
{
	if (_isCollCheck)
	{
		GetCursorPos(&_mousePos);
		ScreenToClient(GET_WINDOW.hwnd, &_mousePos);

		if (PtInRect(&_rect, _mousePos))
		{
			if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
			{
			}
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _collTexture);
		}
		else
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _originalTexture);
	}
}
