#include "pch.h"
#include "ReadyButton.h"
#include "Engine.h"
#include "GameObject.h"
#include "Input.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SceneManager.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

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
				//GET_SINGLE(SceneManager)->SetPlayerReady(mysession->GetPlayerID(), _isReady);

				// Server
				Protocol::C_LOBBY_STATE LobbyStatePkt;
				LobbyStatePkt.set_isready(_isReady);
				LobbyStatePkt.set_playerid(mysession->GetPlayerID());
				LobbyStatePkt.set_playertype((int)GET_SINGLE(SceneManager)->GetPlayerType(mysession->GetPlayerID()));
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(LobbyStatePkt);
				mysession->Send(sendBuffer);
			}
		}
		else
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, _originalTexture);
	}
}
