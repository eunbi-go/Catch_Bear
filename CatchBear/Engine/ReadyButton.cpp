#include "pch.h"
#include "ReadyButton.h"
#include "Engine.h"
#include "GameObject.h"
#include "Input.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SceneManager.h"

ReadyButton::ReadyButton()
{
}

ReadyButton::~ReadyButton()
{
}

void ReadyButton::Update()
{
}

void ReadyButton::LateUpdate()
{
	if (_isCollCheck)
	{
		GetCursorPos(&_mousePos);
		ScreenToClient(GET_WINDOW.hwnd, &_mousePos);

		if (PtInRect(&_rect, _mousePos))
		{
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _collTexture);

			if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
			{
				_isReady = !_isReady;

				// Server
			}
		}
		else
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _originalTexture);
	}
}
