#include "pch.h"
#include "PlayerIcon.h"
#include "Engine.h"
#include "GameObject.h"
#include "Input.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SceneManager.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

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
				GET_SINGLE(SceneManager)->SetPlayerType(mysession->GetPlayerID(), CheckPlayerType(_texKey));

				/////////////// Server///////////////////////
				Protocol::C_LOBBY_STATE pkt;
				pkt.set_playerid(mysession->GetPlayerID());
				pkt.set_playertype((uint64)CheckPlayerType(_texKey));
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
				mysession->Send(sendBuffer);
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
	else if (texKey == L"playerIcon_blue")
	{
		return PLAYER_TYPE::BLUE;
	}
	else if (texKey == L"playerIcon_panda")
	{
		return PLAYER_TYPE::PANDA;
	}
	else if (texKey == L"playerIcon_white")
	{
		return PLAYER_TYPE::WHITE;
	}
	else if (texKey == L"playerIcon_pink")
	{
		return PLAYER_TYPE::PINK;
	}
}
