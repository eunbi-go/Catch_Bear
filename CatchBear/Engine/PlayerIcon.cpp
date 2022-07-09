#include "pch.h"
#include "PlayerIcon.h"
#include "Engine.h"
#include "GameObject.h"
#include "Input.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SceneManager.h"

PlayerIcon::PlayerIcon()
{
}

PlayerIcon::~PlayerIcon()
{
}

void PlayerIcon::Update()
{
}

void PlayerIcon::LateUpdate()
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
				GET_SINGLE(SceneManager)->SetPlayerType(0, CheckPlayerType(_texKey));
			}
		}
		else
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _originalTexture);
	}
}

PLAYER_TYPE PlayerIcon::CheckPlayerType(const wstring& texKey)
{
	if (texKey == L"playerIcon_brown")
	{
		return PLAYER_TYPE::BROWN;
	}
	else if (texKey == L"playerIcon_gray")
	{
		return PLAYER_TYPE::GRAY;
	}
}
