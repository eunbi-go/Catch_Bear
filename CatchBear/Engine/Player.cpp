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
#include "PlayerState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "DashState.h"
#include "DashRestState.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "ItemManager.h"
#include "Item.h"
#include "Light.h"

Player::Player()
{
	// 서버에서 컨트롤하는 플레이어는 _state 갖고있으면 안됨
	_state = new IdleState();
	_curPlayerItem = { false, };
}

Player::~Player()
{
}

void Player::Update()
{
	ApplyItemEffect();
}

void Player::LateUpdate()
{
	// 서버에서 컨트롤하는 플레이어는 서버에서 위치값도 받아오니까 필요없을듯
	KeyCheck();

	////////////////////////////////////////////////////////////////////
	// 이 부분은 직접 플레이하고 있는 플레이어에만 적용되야 함!!
	//PlayerState* state = _player->_state->Update(*shared_from_this());

	//if (state != NULL)
	//{
	//	_player->_state->End(*shared_from_this());
	//	delete _player->_state;
	//	_player->_state = state;
	//	_player->_state->Enter(*shared_from_this());
	//}
	////////////////////////////////////////////////////////////////////

	Vec3 pos = GetTransform()->GetLocalPosition();
	printf("%f, %f, %f\n", pos.x, pos.y, pos.z);

	// 애니메이션 재생하는 부분 -> 모두 적용되야 함
	GetAnimationController()->AdvanceTime(DELTA_TIME);
	GetTransform()->UpdateTransform(NULL);
	GetAnimationController()->SetWorldMatrix();
	
}

void Player::AddPlayerItem(shared_ptr<GameObject> item)
{
	// 플레이어가 갖고있을 수 있는 최대 아이템 수는 3개
	// 아이템을 3개 지니고 있으면 맨처음 아이템 삭제
	if (_playerItemVec.size() < 3)
	{
		_playerItemVec.push_back(item);
	}
}

void Player::KeyCheck()
{
	Protocol::C_MOVE pkt;

	// 게임종료
	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
		::PostQuitMessage(0);

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

	//////////////////////////////////////////////////////////////////////////
	// 이 부분은 직접 플레이하고 있는 플레이어에만 적용되야 함!!
	// State Check
	PlayerState* state = _player->_state->KeyCheck(*shared_from_this());

	if (state != NULL)
	{
		delete _player->_state;
		_player->_state = state;
		_player->_state->Enter(*shared_from_this());
	}
	//////////////////////////////////////////////////////////////////////////

	Move();
	KeyCheck_Item();
}

void Player::Move()
{
	if (_bStunned) return;


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

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::WALK);
		pkt.set_iskeydown(true);
		pkt.set_movedir(0);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}
	else if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos -= _player->GetTransform()->GetLook() * _speed * DELTA_TIME;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::WALK);
		pkt.set_iskeydown(true);
		pkt.set_movedir(1);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}
	else		// 여기서 프레임마다 패킷 보내는게 맘에 안듬 나중에 수정할꺼임
	{
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_iskeydown(false);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}

	Vec3 look = GetTransform()->GetLook();
	int a = 0;

	// 회전
	float delta = 0.f;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		rot.y += DELTA_TIME * _rotSpeed;
		delta = DELTA_TIME * _rotSpeed;

		//GetTransform()->SetLocalRotation(rot);

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

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
	}

	GetTransform()->SetLocalPosition(pos);
	_cameraScript->Revolve(delta, GetTransform()->GetLocalPosition());
}

void Player::KeyCheck_Item()
{
	// 아이템 사용 키입력 - 1, 2, 3
	if (INPUT->GetButtonDown(KEY_TYPE::NUM1))
	{
		if (_playerItemVec.empty()) return;
		UseItem(0);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM2))
	{
		if (_playerItemVec.size() < 1) return;
		UseItem(1);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM3))
	{
		if (_playerItemVec.size() < 2) return;
		UseItem(2);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::TEST_KEY))
		SlowDown();
}

void Player::UseItem(int itemNum)
{
	// 키입력을 받은 후 _curPlayerItem의 bool값을 true로 만들어주는 함수
	ITEM_EFFECT itemEffect = static_pointer_cast<Item>(_playerItemVec[itemNum]->GetScript(0))->GetItemEffect();

	switch (itemEffect)
	{
	case ITEM_EFFECT::SPEED_UP:
		_curPlayerItem[Player::ITEM::SPEED_UP] = true;
		break;
	case ITEM_EFFECT::TELEPORT:
		_curPlayerItem[Player::ITEM::TELEPORT] = true;
		break;
	case ITEM_EFFECT::SHIELD:
		_curPlayerItem[Player::ITEM::SHIELD] = true;
		break;
	case ITEM_EFFECT::SPEED_DOWN:
		// 다른 플레이어들의 속도 감소시켜야함
		_curPlayerItem[Player::ITEM::SPEED_DOWN] = true;	// test
		Item_SpeedDown();
		break;
	case ITEM_EFFECT::BLIND:
		// 다른 플레이어들의 시야 흐리게
		_curPlayerItem[Player::ITEM::BLIND] = true;	// test
		Item_Blind();
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		_curPlayerItem[Player::ITEM::DEBUFF_OFF] = true;	// test
		Item_DebuffOff();
		break;
	case ITEM_EFFECT::STUN:
		// 다른 플레이어들 스턴걸기
		_curPlayerItem[Player::ITEM::STUN] = true;	// test
		Item_Stun();
		break;
	}

	_playerItemVec.erase(_playerItemVec.begin() + itemNum);
}

void Player::ApplyItemEffect()
{
	// _curPlayerItem에서 true인 아이템 효과를 플레이어에 적용시켜주는 함수, Update에서 매번 호출

	if (_curPlayerItem[Player::ITEM::SPEED_UP])
		Item_SpeedUp();

	if (_curPlayerItem[Player::ITEM::TELEPORT])
		Item_Teleport();

	if (_curPlayerItem[Player::ITEM::SHIELD])
		Item_Shield();

	if (_curPlayerItem[Player::ITEM::SPEED_DOWN])
		SlowDown();

	if (_curPlayerItem[Player::ITEM::BLIND])
		Blinded();

	if (_curPlayerItem[Player::ITEM::DEBUFF_OFF])
		Item_DebuffOff();

	if (_curPlayerItem[Player::ITEM::STUN])
		Stunned();
}

void Player::Item_SpeedUp()
{
	// 스피드 변경 전 한번만 하도록
	if (_speed == _originalSpeed)
	{
		_state->End(*shared_from_this());
		delete _state;
		_state = new DashState;
		_state->Enter(*shared_from_this());
	}
}

void Player::Item_Teleport()
{
	// 카메라랑 플레이어 지금 방향의 앞으로 순간이동
	Vec3 pos = GetTransform()->GetLocalPosition();
	pos += GetTransform()->GetLook() * _fTeleportDis;
	GetTransform()->SetLocalPosition(pos);

	_curPlayerItem[Player::ITEM::TELEPORT] = false;
}

void Player::Item_Shield()
{
	// 쉴드 상태라면 디버프 방어
	// 쉴드 상태때 디버프 1회 방해했으면 쉴드 해제
	_fShieldTime += DELTA_TIME;
	
	if (_fShieldTime <= 5.f)
	{
		if (!_bUseShield)
		{
			_curPlayerItem[Player::ITEM::SPEED_DOWN] = false;
			_curPlayerItem[Player::ITEM::BLIND] = false;
			_curPlayerItem[Player::ITEM::STUN] = false;

			_bUseShield = true;
		}
	}
	else if (_bUseShield || _fShieldTime > 5.f)
	{
		_fShieldTime = 0.f;
		_curPlayerItem[Player::ITEM::SHIELD] = false;
		_bUseShield = false;
	}
}

void Player::Item_SpeedDown()
{
	// 다른 플레이어들 속도 감소시키기 위한 함수
	// 서버에서 해야하나 ?
	// 클라에서 함수가 따로 필요 없으면 삭제함
}

void Player::Item_Blind()
{
}

void Player::Item_DebuffOff()
{
	// 유니크 아이템 - 자신에게 걸려있는 모든 디버프 해제
	// 디버프: SPEED_DOWN, BLIND, STUN
	_curPlayerItem[Player::ITEM::SPEED_DOWN] = false;
	_curPlayerItem[Player::ITEM::BLIND] = false;
	_curPlayerItem[Player::ITEM::STUN] = false;

	_curPlayerItem[Player::ITEM::DEBUFF_OFF] = false;
}

void Player::Item_Stun()
{
}

void Player::SlowDown()
{
	// 자신 제외 모든 플레이어 5초동안 속도 감소
	if (_speed == _originalSpeed)
	{
		_state->End(*shared_from_this());
		delete _state;
		_state = new MoveState;
		_state->Enter(*shared_from_this());
	}
}

void Player::Blinded()
{
	_fBlindTime += DELTA_TIME;

	// 연막, 씬 안의 조명을 가져와서 어둡게?
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	auto& lights = scene->GetLights();

	for (auto& light : lights)
		static_pointer_cast<Light>(light)->SetDiffuse(Vec3(0.05f, 0.05f, 0.05f));

	if (_fBlindTime > 5.f)
	{
		for (auto& light : lights)
			static_pointer_cast<Light>(light)->SetDiffuse(Vec3(1.f, 1.f, 1.f));

		_fBlindTime = 0.f;
		_curPlayerItem[Player::ITEM::BLIND] = false;
	}
}

void Player::Stunned()
{
	// 유니크 아이템 - 3초간 스턴
	_fStunTime += DELTA_TIME;

	if (_fStunTime <= 300.f)
	{
		_bStunned = true;
	}
	else
	{
		_bStunned = false;
		_fStunTime = 0.f;
		_curPlayerItem[Player::ITEM::STUN] = false;
	}
}
