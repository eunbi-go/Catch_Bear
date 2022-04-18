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
#include "SlowRestState.h"
#include "SlowState.h"
#include "StunState.h"

#include "ItemManager.h"
#include "Item.h"
#include "Light.h"
#include "ItemSlotManager.h"
#include "TagMark.h"

Protocol::C_MOVE pkt;

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
	//if (_bStunned) return;
	// 서버에서 컨트롤하는 플레이어는 서버에서 위치값도 받아오니까 필요없을듯
	KeyCheck();

	////////////////////////////////////////////////////////////////////
	// 이 부분은 직접 플레이하고 있는 플레이어에만 적용되야 함!!
	PlayerState* state = _state->Update(*_player, _curState);
	_player->_curState = _curState;

	if (state != NULL)
	{
		_state->End(*_player);
		delete _state;
		_state = state;
		_state->Enter(*_player);
	}

#pragma region 애니메이션동기화
	switch (_player->_curState)
	{
	case STATE::WALK:
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::WALK);
		break;
	case STATE::JUMP:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::JUMP);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::ATTACK:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::ATTACK);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::STUN:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::STUN);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::DASH:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::DASH);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	}
#pragma endregion 애니메이션동기화
	////////////////////////////////////////////////////////////////////

	/*Vec3 pos = GetTransform()->GetLocalPosition();
	printf("%f, %f, %f\n", pos.x, pos.y, pos.z);*/

	// 애니메이션 재생하는 부분 -> 모두 적용되야 함
	GetAnimationController()->AdvanceTime(DELTA_TIME);
	GetTransform()->UpdateTransform(NULL);
	GetAnimationController()->SetWorldMatrix();
	
	Vec3 trans = GetTransform()->GetLocalPosition();
	//printf("%f, %f, %f\n", trans.x, trans.y, trans.z);
	
}

void Player::AddPlayerItem(shared_ptr<GameObject> item)
{
	// 플레이어가 갖고있을 수 있는 최대 아이템 수는 3개
	// 아이템을 3개 지니고 있으면 아이템 획득해도 무시
	if (_playerItemVec.size() < 3)
	{
		_playerItemVec.push_back(item);

		// 해당 아이템을 확인해서 slot에 맞는 텍스처를 설정해야 한다.
		ITEM_EFFECT itemType = static_pointer_cast<Item>(item->GetScript(0))->GetItemEffect();
		GET_SINGLE(ItemSlotManager)->AddItem(itemType);
	}
}

void Player::KeyCheck()
{
	// 게임종료
	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
		::PostQuitMessage(0);

	

	if (mysession == NULL)
		return;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_player = scene->GetPlayer(mysession->GetPlayerID());
	Vec3 pos = _player->GetTransform()->GetLocalPosition();
	//////////////////////////////////////////////////////////////////////////
	// 이 부분은 직접 플레이하고 있는 플레이어에만 적용되야 함!!
	// State Check
	PlayerState* state = _state->KeyCheck(*_player, _curState);

	if (_bStunned) return;		// 멀티플레이 환경에서 stun 상태일때 WALK애니메이션 하지 않게 함

	_player->_curState = _curState;

	if (state != NULL)
	{
		_state->End(*_player);
		delete _state;
		_state = state;
		_state->Enter(*_player);
	}

#pragma region 애니메이션동기화
	if (_player->GetPlayerID() != mysession->GetPlayerID())	//디버깅용
		int a = 10;

	switch (_player->_curState)
	{
	case STATE::IDLE:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::IDLE);
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		if (gPacketControl % 80 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::JUMP:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::JUMP);
		if (gPacketControl % 10 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::ATTACK:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::ATTACK);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::STUN:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::STUN);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::DASH:
	{
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::DASH);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	}
#pragma endregion 애니메이션동기화
	//////////////////////////////////////////////////////////////////////////

	Move();
	KeyCheck_Item();
}

static bool isFirstEnter = true;

void Player::Move()
{
	if (_bStunned) return;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
	shared_ptr<GameObject> tagObject = scene->GetGameObject(L"PlayerTag1");

	Vec3 pos = _player->GetTransform()->GetLocalPosition();
	Vec3 rot = _player->GetTransform()->GetLocalRotation();

	// 이동 오류때문에 시작할때 stun 패킷 한번 보내고 시작
	if (isFirstEnter) {
		Item_Stun();
		isFirstEnter = false;
	}

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
		if (_player->GetIsAllowPlayerMove())
			pos += _player->GetTransform()->GetLook() * _speed * DELTA_TIME;
		
		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_movedir(0);

		if (gPacketControl % 3 == 1)
		{
			if (_player->GetIsAllowPlayerMove()) {
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
				mysession->Send(sendBuffer);
			}
		}
		
	}
	else if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos -= _player->GetTransform()->GetLook() * _speed * DELTA_TIME;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_movedir(1);

		if (gPacketControl % 3 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
	}

	// 회전
	float delta = 0.f;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		rot.y += DELTA_TIME * _rotSpeed;
		delta = DELTA_TIME * _rotSpeed;

		Vec3 pl = _player->GetTransform()->GetLook();
		cout << "플레이어 " << _player->GetPlayerID() << " look: " <<
			pl.x << ", " << pl.y << ", " << pl.z << endl;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		if (gPacketControl % 3 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		_player->GetTransform()->SetLocalRotation(rot);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		rot.y -= DELTA_TIME * _rotSpeed;
		delta = -DELTA_TIME * _rotSpeed;

		Vec3 pl = _player->GetTransform()->GetLook();
		cout << "플레이어 " << _player->GetPlayerID() << " look: " <<
			pl.x << ", " << pl.y << ", " << pl.z << endl;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		if (gPacketControl % 3 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}

		_player->GetTransform()->SetLocalRotation(rot);
	}

	_player->GetTransform()->SetLocalPosition(pos);
	_cameraScript->Revolve(delta, _player->GetTransform()->GetLocalPosition());
	static_pointer_cast<TagMark>(tagObject->GetScript(0))->SetPosition(pos);
}

void Player::KeyCheck_Item()
{
	// 아이템 사용 키입력 - 1, 2, 3
	if (INPUT->GetButtonDown(KEY_TYPE::NUM1))
	{
		if (_playerItemVec.size() < 1)
			return;

		UseItem(0);
		GET_SINGLE(ItemSlotManager)->UseItem(1);
		DeletePlayerItem(0);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM2))
	{
		if (_playerItemVec.size() < 2) 
			return;

		UseItem(1);
		GET_SINGLE(ItemSlotManager)->UseItem(2);
		DeletePlayerItem(1);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM3))
	{
		if (_playerItemVec.size() < 3) 
			return;
		
		UseItem(2);
		GET_SINGLE(ItemSlotManager)->UseItem(3);
		DeletePlayerItem(2);
	}

	//if (INPUT->GetButtonDown(KEY_TYPE::TEST_KEY))
	//	SlowDown();
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
		//_curPlayerItem[Player::ITEM::SPEED_DOWN] = true;	// test
		Item_SpeedDown();
		break;
	case ITEM_EFFECT::BLIND:
		// 다른 플레이어들의 시야 흐리게
		//_curPlayerItem[Player::ITEM::BLIND] = true;	// test
		Item_Blind();
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		_curPlayerItem[Player::ITEM::DEBUFF_OFF] = true;	// test
		Item_DebuffOff();
		break;
	case ITEM_EFFECT::STUN:
		// 다른 플레이어들 스턴걸기
		//_curPlayerItem[Player::ITEM::STUN] = true;	// test
		Item_Stun();
		break;
	}

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

void Player::DeletePlayerItem(int itemIndex)
{
	_playerItemVec.erase(_playerItemVec.begin() + itemIndex);
}

void Player::Item_SpeedUp()
{
	// 스피드 변경 전 한번만 하도록
	if (_speed == _originalSpeed)
	{
		_state->End(*_player);
		delete _state;_state = new DashState;
		_state->Enter(*_player);

		pkt.set_playerid(mysession->GetPlayerID());
		pkt.set_state(Protocol::DASH);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
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
	// 밑에서 위로 뭔가 올라오는 파티클 효과 추가 - 쉴드 상태인걸 알 수 있도록
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
	/////////////// prod by. wc ///////////////
	Protocol::C_USE_DEBUFITEM pkt;
	pkt.set_fromplayerid(mysession->GetPlayerID());
	pkt.set_itemtype(Protocol::DEBUF_SPEEDDOWN);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	mysession->Send(sendBuffer);
	///////////////////////////////////////////
}

void Player::Item_Blind()
{
	/////////////// prod by. wc ///////////////
	Protocol::C_USE_DEBUFITEM pkt;
	pkt.set_fromplayerid(mysession->GetPlayerID());
	pkt.set_itemtype(Protocol::DEBUF_BLIND);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	mysession->Send(sendBuffer);
	///////////////////////////////////////////
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
	/////////////// prod by. wc ///////////////
	Protocol::C_USE_STUN pkt;
	pkt.set_fromplayerid(mysession->GetPlayerID());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	mysession->Send(sendBuffer);
	///////////////////////////////////////////
}

void Player::SlowDown()
{
	// 자신 제외 모든 플레이어 5초동안 속도 감소
	if (_speed == _originalSpeed)
	{
		_state->End(*GetGameObject());
		delete _state;
		_state = new SlowState;
		_state->Enter(*GetGameObject());
	}
}

void Player::Blinded()
{
	_fBlindTime += DELTA_TIME;

	// 연막, 씬 안의 조명을 가져와서 어둡게?
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	auto& lights = scene->GetLights();

	for (auto& light : lights)
	{
		static_pointer_cast<Light>(light)->SetDiffuse(Vec3(0.f, 0.f, 0.f));
		static_pointer_cast<Light>(light)->SetAmbient(Vec3(0.01f, 0.01f, 0.01f));
		static_pointer_cast<Light>(light)->SetSpecular(Vec3(0.01f, 0.01f, 0.01f));
	}

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
	if (!_bStunned)
	{
		_bStunned = true;

		if (!isFirstEnter) {
			_state->End(*_player);
			delete _state;
			_state = new StunState;
			_state->Enter(*_player);
		}
	}
}
