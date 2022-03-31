#include "pch.h"
#include "Player.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CameraScript.h"
#include "AnimationController.h"
#include "AnimationTrack.h"
#include "ServerPacketHandler.h"
#include "Session.h"
#include "ServerSession.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::LateUpdate()
{
	KeyCheck();
	StateCheck();
	AnimationCheck();

	GetAnimationController()->AdvanceTime(DELTA_TIME);
	GetTransform()->UpdateTransform(NULL);
	GetAnimationController()->SetWorldMatrix();
}

void Player::KeyCheck()
{
	Protocol::C_MOVE pkt;

	// 게임종료
	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
		::PostQuitMessage(0);	

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	//_player = GetGameObject();
	if (mysession == NULL)
		return;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player" && gameObject->GetPlayerID() == mysession->GetPlayerID())
		{
			_player = gameObject;
			break;
		}
	}
	Vec3 pos = _player->GetTransform()->GetLocalPosition();
	Vec3 rot = _player->GetTransform()->GetLocalRotation();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Main_Camera")
		{
			_camera = gameObject;
			break;
		}
	}

	_cameraScript = static_pointer_cast<CameraScript>(_camera->GetScript(0));
	
	// 이동
	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos += _player->GetTransform()->GetLook() * _speed * DELTA_TIME;
		//_curState = WALK;		
		//_player->Set_CurState(WALK); 

		// 서버로 MOVE패킷 전송
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::WALK);
		pkt.set_movedir(0);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}

	else if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos -= _player->GetTransform()->GetLook() * _speed * DELTA_TIME;
		//_player->Set_CurState(WALK);

		// 서버로 MOVE패킷 전송
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::WALK);		
		pkt.set_movedir(1);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}
	else
	{
		//_player->Set_CurState(IDLE);
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::IDLE);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}
	// 회전
	float delta = 0.f;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		rot.y += DELTA_TIME * _rotSpeed;
		delta = DELTA_TIME * _rotSpeed;

		//GetTransform()->SetLocalRotation(rot);
		
		// 이동+회전: WALK
		if (_player->Get_CurState() != WALK)	_player->Set_CurState(IDLE);

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		rot.y -= DELTA_TIME * _rotSpeed;
		delta = -DELTA_TIME * _rotSpeed;

		//GetTransform()->SetLocalRotation(rot);

		// 이동+회전: WALK
		if (_player->Get_CurState() != WALK)	_player->Set_CurState(IDLE);

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}

	if (INPUT->GetButton(KEY_TYPE::SPACE))
	{
		_player->Set_CurState(JUMP);
	}
	
	//GetTransform()->SetLocalPosition(pos);
	_cameraScript->Revolve(delta, _player->GetTransform()->GetLocalPosition());
}

void Player::StateCheck()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	if (mysession == NULL)
		return;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player" && gameObject->GetPlayerID() == mysession->GetPlayerID())
		{
			_player = gameObject;
			break;
		}
	}
	if (_player->Get_CurState() != _player->Get_PreState())
	{
		switch (_player->Get_CurState())
		{
		case IDLE:
			_player->GetAnimationController()->SetTrackAnimationSet(0, 0);
			break;
		case WALK:
			_player->GetAnimationController()->SetTrackAnimationSet(0, 1);
			break;
		case DASH:
			_player->GetAnimationController()->SetTrackAnimationSet(0, 3);
			break;
		case JUMP:
			_player->GetAnimationController()->SetTrackAnimationSet(0, 2);
			break;
		case ATTACK:
			_player->GetAnimationController()->SetTrackAnimationSet(0, 4);
			break;
		case END:
			break;
		default:
			break;
		}

		_player->Set_PreState(_player->Get_CurState());
	}
}

void Player::AnimationCheck()
{
	if (_player->Get_CurState() == DASH || _player->Get_CurState() == JUMP || _player->Get_CurState() == ATTACK)
	{
		if (GetAnimationController()->IsAnimationFinish(0))
			_player->Set_CurState(IDLE);
	}
}
