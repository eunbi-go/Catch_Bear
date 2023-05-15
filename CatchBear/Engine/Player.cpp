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
#include "MeshRenderer.h"
#include "Resources.h"
#include "Engine.h"
#include "SoundManager.h"
#include "ShieldParticleManager.h"

Protocol::C_MOVE pkt;
Protocol::C_STATE StatePkt;

Player::Player()
{
	// �������� ��Ʈ���ϴ� �÷��̾�� _state ���������� �ȵ�
	_state = new IdleState();
	_curPlayerItem = { false, };
	_playerItemArr = { Item::ITEM_EFFECT::NONE, Item::ITEM_EFFECT::NONE, Item::ITEM_EFFECT::NONE };
}

Player::~Player()
{
}

void Player::Update()
{
	//if (!_isInit)
	//{
	//	_isInit = true;
	//	GET_SINGLE(ItemSlotManager)->ResetItemSlot(1);
	//	GET_SINGLE(ItemSlotManager)->ResetItemSlot(2);
	//	GET_SINGLE(ItemSlotManager)->ResetItemSlot(3);
	//}
	//cout << "�÷��̾� " << _player->GetPlayerID() << ": " << /*static_pointer_cast<Player>(_player->GetScript(0))->*/_iScore << endl;
	ApplyItemEffect();
}

void Player::LateUpdate()
{
	PlayerState* state = _state->Update(*_player, _curStatePlayer);
	_player->_curState = _curStatePlayer;

	// ���°� �ٸ� ��� ���� ��ȭ.
	if (state != NULL)
	{
		_state->End(*_player);
		delete _state;
		_state = state;
		_state->Enter(*_player);
	}


	//cout << _player->GetPlayerID() << " : " << _curStatePlayer << endl;


	// �������� ��Ʈ���ϴ� �÷��̾�� �������� ��ġ���� �޾ƿ��ϱ� �ʿ������
	KeyCheck();

	////////////////////////////////////////////////////////////////////
	// �� �κ��� ���� �÷����ϰ� �ִ� �÷��̾�� ����Ǿ� ��!!
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_player = scene->GetPlayer(mysession->GetPlayerID());



	// �ִϸ��̼� ����ϴ� �κ� -> ��� ����Ǿ� ��
	GetAnimationController()->AdvanceTime(DELTA_TIME);
	GetTransform()->UpdateTransform(NULL);
	GetAnimationController()->SetWorldMatrix();

	Vec3 trans = GetTransform()->GetLocalPosition();
	//printf("%f, %f, %f\n", trans.x, trans.y, trans.z);

	if (_player->_curState == WALK && GetAnimationController()->GetCurrentFrame() >= _nWalkSoundFrame)
	{
		GET_SINGLE(SoundManager)->PlaySound(L"walk.mp3", SoundManager::MOVE);
	}
}

void Player::AddPlayerItem(Item::ITEM_EFFECT itemEffect)
{
	// �÷��̾ �������� �� �ִ� �ִ� ������ ���� 3��
	// �������� 3�� ���ϰ� ������ ������ ȹ���ص� ����
	if (_iItemCnt < 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (_playerItemArr[i] == Item::ITEM_EFFECT::NONE)
			{
				_playerItemArr[i] = itemEffect;
				printf("%d ȹ��\n", itemEffect);

				// �ش� �������� Ȯ���ؼ� slot�� �´� �ؽ�ó�� �����ؾ� �Ѵ�.
				GET_SINGLE(ItemSlotManager)->AddItem(itemEffect);

				_iItemCnt++;	// ������ ������ �ö󰡰� ����ϸ� ������

				break;
			}
		}
	}
}

void Player::Reset()
{
	// �÷��̾� ���� ������
	_playerItemArr[0] = Item::ITEM_EFFECT::NONE;
	_playerItemArr[1] = Item::ITEM_EFFECT::NONE;
	_playerItemArr[2] = Item::ITEM_EFFECT::NONE;

	GET_SINGLE(ItemSlotManager)->ResetItemSlot(1);
	GET_SINGLE(ItemSlotManager)->ResetItemSlot(2);
	GET_SINGLE(ItemSlotManager)->ResetItemSlot(3);

	// �÷��̾� �������� ����,����� ������ �ʱ�ȭ
	_curPlayerItem = { false, };

	// �÷��̾� ���� �ʱ�ȭ, ���� �õ��� �ϴ°Ÿ� �װ� ���� ���ϰ� ������ҵ�
	_iScore = 0;

	// �÷��̾� ������ ���� �ʱ�ȭ
	_iItemCnt = 0;

	// �÷��̾� ��������� �ʱ�ȭ (Ȥ�� �𸣴� �ص�)
	_speed = 10.f;
	//_bStunned = false;	// �̵� �������� �ϴ� ����
	_fShieldTime = 0.f;
	_fBlindTime = 0.f;
}

static bool isFirstEnter = true;

void Player::KeyCheck()
{
	// ��������
	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
		::PostQuitMessage(0);

	if (mysession == NULL)
		return;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_player = scene->GetPlayer(mysession->GetPlayerID());
	Vec3 pos = _player->GetTransform()->GetLocalPosition();
	//////////////////////////////////////////////////////////////////////////
	// �� �κ��� ���� �÷����ϰ� �ִ� �÷��̾�� ����Ǿ� ��!!
	// State Check
	PlayerState* state = _state->KeyCheck(*_player, _curStatePlayer);

	// Item KeyCheck /////////////////////////////////////////////////////////
	// stunned�� Ű�Է��� �ȹ޾Ƽ� �����϶� �˾��� �ȵ�, �ڵ� ���� �ű�
	KeyCheck_Item();
	KeyCheck_Cheat();
	//////////////////////////////////////////////////////////////////////////

	//cout << _bStunned << endl;

	if (_bStunned)	// ��Ƽ�÷��� ȯ�濡�� stun �����϶� WALK�ִϸ��̼� ���� �ʰ� ��
	{
		_curStatePlayer = STATE::STUN;
		//_bStunned = false;
		return;
	}


	_player->_curState = _curStatePlayer;

	if (state != NULL)
	{
		_state->End(*_player);
		delete _state;
		_state = state;
		_state->Enter(*_player);
	}

#pragma region �ִϸ��̼ǵ���ȭ
	switch (_player->_curState)
	{
	case STATE::IDLE:
	{
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::IDLE);
		if (gPacketControl % 10 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::WALK:
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::WALK);
		if (gPacketControl % 2 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	case STATE::JUMP:
	{
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::JUMP);
		if (gPacketControl % 10 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::ATTACK:
	{
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::ATTACK);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::DASH:
	{
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::DASH);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	case STATE::SLOW:
	{
		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::SLOW);
		if (gPacketControl % 60 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
			mysession->Send(sendBuffer);
		}
		break;
	}
	}
#pragma endregion �ִϸ��̼ǵ���ȭ
	//////////////////////////////////////////////////////////////////////////

	Move();
}



void Player::Move()
{
	if (_bStunned)	return;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	shared_ptr<GameObject> tagObject;
	if (mysession->GetPlayerID() == 0)
		tagObject = scene->GetGameObject(L"PlayerTag1");
	if (mysession->GetPlayerID() == 1)
		tagObject = scene->GetGameObject(L"PlayerTag2");
	if (mysession->GetPlayerID() == 2)
		tagObject = scene->GetGameObject(L"PlayerTag3");

	Vec3 pos = _player->GetTransform()->GetLocalPosition();
	Vec3 rot = _player->GetTransform()->GetLocalRotation();

	
	if (isFirstEnter) {
		Item_Stun();
		isFirstEnter = false;

		// �ǵ� ���� �빮�� �����Ҷ� �ǵ� ��Ŷ �ѹ� ������ ����
		Protocol::C_USE_SHIELD pkt;
		pkt.set_playerid(mysession->GetPlayerID());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);

		if (mysession->GetPlayerID() == g_EnterPlayerCnt - 1)
			scene->_FinalPlayerEnter = true;		
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

	// �̵�
	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		if (_player->GetIsAllowPlayerMove())
			pos += _player->GetTransform()->GetLook() * _speed * DELTA_TIME;
		else
		{
			if (_dir == DIR::DIR_LEFT)
				pos -= _player->GetTransform()->GetRight() * (_speed / 5.0f) * DELTA_TIME;
			else
				pos += _player->GetTransform()->GetRight() * (_speed / 5.0f) * DELTA_TIME;
		}

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		//if (gPacketControl % 2 == 1)
		//{
			if (_player->GetIsAllowPlayerMove()) {
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
				mysession->Send(sendBuffer);
			}
		//}
		
	}
	else if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos -= _player->GetTransform()->GetLook() * _speed * DELTA_TIME;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		//if (gPacketControl % 2 == 1)
		//{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		//}
	}

	// ȸ��
	float delta = 0.f;
	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		_dir = DIR::DIR_RIGHT;

		rot.y += DELTA_TIME * _rotSpeed;
		delta = DELTA_TIME * _rotSpeed;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		if (gPacketControl % 5 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}
		_player->GetTransform()->SetLocalRotation(rot);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		_dir = DIR::DIR_LEFT;

		rot.y -= DELTA_TIME * _rotSpeed;
		delta = -DELTA_TIME * _rotSpeed;

		pkt.set_xpos(pos.x);
		pkt.set_ypos(pos.y);
		pkt.set_zpos(pos.z);
		pkt.set_yrot(rot.y);
		pkt.set_playerid(mysession->GetPlayerID());

		if (gPacketControl % 5 == 1)
		{
			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
			mysession->Send(sendBuffer);
		}

		_player->GetTransform()->SetLocalRotation(rot);
	}


	_player->GetTransform()->SetLocalPosition(pos);
	_cameraScript->Revolve(delta, _player->GetTransform()->GetLocalPosition());
	static_pointer_cast<TagMark>(tagObject->GetScript(0))->SetPosition(pos);
	static_pointer_cast<TagMark>(tagObject->GetScript(0))->SetRotation();
}

void Player::KeyCheck_Item()
{
	// ħ�������̸� ������ ��� X
	if (_curPlayerItem[Player::ITEM::SILENCE])
		return;

	// ������ ��� Ű�Է� - 1, 2, 3
	if (INPUT->GetButtonDown(KEY_TYPE::NUM1))
	{
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE) return;
		if (CheckDebuff(_playerItemArr[0])) return;

		UseItem(0);
		GET_SINGLE(ItemSlotManager)->UseItem(1);
		GET_SINGLE(SoundManager)->PlaySound(L"item1.MP3", SoundManager::CHANNELID::ITEM);
		DeletePlayerItem(0);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM2))
	{
		if (_playerItemArr[1] == Item::ITEM_EFFECT::NONE) return;
		if (CheckDebuff(_playerItemArr[1])) return;

		UseItem(1);
		GET_SINGLE(ItemSlotManager)->UseItem(2);
		GET_SINGLE(SoundManager)->PlaySound(L"item1.MP3", SoundManager::CHANNELID::ITEM);
		DeletePlayerItem(1);
	}
	if (INPUT->GetButtonDown(KEY_TYPE::NUM3))
	{
		if (_playerItemArr[2] == Item::ITEM_EFFECT::NONE) return;
		if (CheckDebuff(_playerItemArr[2])) return;

		UseItem(2);
		GET_SINGLE(ItemSlotManager)->UseItem(3);
		GET_SINGLE(SoundManager)->PlaySound(L"item1.MP3", SoundManager::CHANNELID::ITEM);
		DeletePlayerItem(2);
	}
}

void Player::UseItem(int itemNum)
{
	// ħ�������̸� ������ ��� X
	if (_curPlayerItem[Player::ITEM::SILENCE])
		return;

	// Ű�Է��� ���� �� _curPlayerItem�� bool���� true�� ������ִ� �Լ�
	Item::ITEM_EFFECT itemEffect = _playerItemArr[itemNum];

	printf("����� ������: %d\n", itemEffect);

	switch (itemEffect)
	{
	case Item::ITEM_EFFECT::SPEED_UP:
		_curPlayerItem[Player::ITEM::SPEED_UP] = true;
		break;
	case Item::ITEM_EFFECT::TELEPORT:
		_curPlayerItem[Player::ITEM::TELEPORT] = true;
		break;
	case Item::ITEM_EFFECT::SHIELD:
	{
		_curPlayerItem[Player::ITEM::SHIELD] = true;
		_shieldPlayerIdx = mysession->GetPlayerID();

		// Server
		Protocol::C_USE_SHIELD pkt;
		pkt.set_playerid(mysession->GetPlayerID());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		mysession->Send(sendBuffer);
		break;
	}
	case Item::ITEM_EFFECT::SPEED_DOWN:
		//_curPlayerItem[Player::ITEM::SPEED_DOWN] = true;	// test
		Item_SpeedDown();
		break;
	case Item::ITEM_EFFECT::BLIND:
		//_curPlayerItem[Player::ITEM::BLIND] = true;	// test
		Item_Blind();
		break;
	case Item::ITEM_EFFECT::SILENCE:
		//_curPlayerItem[Player::ITEM::SILENCE] = true;	// test
		Item_Silence();
		break;
	case Item::ITEM_EFFECT::STUN:
		//_curPlayerItem[Player::ITEM::STUN] = true;	// test
		Item_Stun();
		break;
	}
}

void Player::ApplyItemEffect()
{
	// _curPlayerItem���� true�� ������ ȿ���� �÷��̾ ��������ִ� �Լ�, Update���� �Ź� ȣ��

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

	if (_curPlayerItem[Player::ITEM::SILENCE])
		Silence();

	if (_curPlayerItem[Player::ITEM::STUN])
		Stunned();
}

void Player::DeletePlayerItem(int itemIndex)
{
	_playerItemArr[itemIndex] = Item::ITEM_EFFECT::NONE;
	_iItemCnt--;
}

void Player::ClearDebuff()
{
	// ����� ������ ���忡 ����

	// SPEED_DOWN ����
	if (_curPlayerItem[Player::ITEM::SPEED_DOWN])
	{
		_state->End(*_player);
		delete _state;
		_state = new IdleState;
		_state->Enter(*_player);

		_speed = 10.f;
		_curPlayerItem[Player::ITEM::SPEED_DOWN] = false;
		cout << "SpeedDown ����" << endl;
	}

	// BLIND ����
	if (_curPlayerItem[Player::ITEM::BLIND])
	{
		shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
		auto& lights = scene->GetLights();

		for (auto& light : lights)
		{
			static_pointer_cast<Light>(light)->SetDiffuse(Vec3(1.f, 1.f, 1.f));
			static_pointer_cast<Light>(light)->SetAmbient(Vec3(0.3f, 0.3f, 0.3f));
			static_pointer_cast<Light>(light)->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		}

		_fBlindTime = 0.f;
		_curPlayerItem[Player::ITEM::BLIND] = false;
		cout << "Blind ����" << endl;
	}

	// STUN ����
	if (_curPlayerItem[Player::ITEM::STUN])
	{
		_state->End(*_player);
		delete _state;
		_state = new IdleState;
		_state->Enter(*_player);

		_curPlayerItem[Player::ITEM::STUN] = false;
		_bStunned = false;
		cout << "Stun ����" << endl;
	}
}

bool Player::CheckShield()
{
	if (_shieldPlayerIdx != mysession->GetPlayerID())	// �ǵ� �� �÷��̾ �ƴϸ� üũ��������
		return false;

	if (_curPlayerItem[Player::ITEM::SHIELD])
	{
		_curPlayerItem[Player::ITEM::SHIELD] = false;
		_fShieldTime = 0.f;
		GET_SINGLE(ItemSlotManager)->UseShieldItem();
		GET_SINGLE(ShieldParticleManager)->SetShieldParticleOff();	// ���� ��ƼŬ ȿ�� ����
		return true;
	}

	else
		return false;
}

bool Player::CheckDebuff(Item::ITEM_EFFECT itemEffect)
{
	// ������ ��� Űüũ ���� ȣ��
	// ����� ȿ���� �޴� ���̶��, ���� ������ ��� �Ұ��� -> true ����
	if (_curPlayerItem[ITEM::SPEED_DOWN] || _curPlayerItem[ITEM::BLIND] || _curPlayerItem[ITEM::STUN])
	{
		if (itemEffect == Item::ITEM_EFFECT::SPEED_UP ||
			itemEffect == Item::ITEM_EFFECT::TELEPORT ||
			itemEffect == Item::ITEM_EFFECT::SHIELD ||
			itemEffect == Item::ITEM_EFFECT::SILENCE ||
			itemEffect == Item::ITEM_EFFECT::STUN ||
			itemEffect == Item::ITEM_EFFECT::SPEED_DOWN ||
			itemEffect == Item::ITEM_EFFECT::BLIND)
		{
			cout << "������� - ���������� ��� ����" << endl;
			return true;
		}
	}

	return false;
}

void Player::KeyCheck_Cheat()
{
	// ���� ������ Ȯ��
	if (INPUT->GetButtonDown(KEY_TYPE::TEST_KEY))	// T
	{
		printf("1�� ������: %d\n", _playerItemArr[0]);
		printf("2�� ������: %d\n", _playerItemArr[1]);
		printf("3�� ������: %d\n", _playerItemArr[2]);
	}

	// ġƮŰ
	if (INPUT->GetButtonDown(KEY_TYPE::NUM4))	// ������ â ���
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::NONE;
		_playerItemArr[1] = Item::ITEM_EFFECT::NONE;
		_playerItemArr[2] = Item::ITEM_EFFECT::NONE;

		GET_SINGLE(ItemSlotManager)->ResetItemSlot(1);
		GET_SINGLE(ItemSlotManager)->ResetItemSlot(2);
		GET_SINGLE(ItemSlotManager)->ResetItemSlot(3);

		_iItemCnt = 0;
	}

	// �ӵ� ������� & speedUp speedDown ����
	if (INPUT->GetButtonDown(KEY_TYPE::F))
	{
		_speed = 10.f;
		_curPlayerItem[ITEM::SPEED_UP] = false;
		_curPlayerItem[ITEM::SPEED_DOWN] = false;

		_state->End(*GetGameObject());
		delete _state;
		_state = new IdleState;
		_state->Enter(*GetGameObject());
	}

	// ������ ���� ġƮŰ
	if (INPUT->GetButtonDown(KEY_TYPE::Z))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::SPEED_UP;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::SPEED_UP);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::X))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::TELEPORT;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::TELEPORT);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::C))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::SHIELD;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::SHIELD);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::V))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::SILENCE;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::SILENCE);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::B))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::STUN;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::STUN);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::N))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::SPEED_DOWN;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::SPEED_DOWN);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}
	if (INPUT->GetButtonDown(KEY_TYPE::M))
	{
		_playerItemArr[0] = Item::ITEM_EFFECT::BLIND;
		GET_SINGLE(ItemSlotManager)->AddItem_Cheat(Item::ITEM_EFFECT::BLIND);
		if (_playerItemArr[0] == Item::ITEM_EFFECT::NONE)
			_iItemCnt++;
	}

	// ������� ������ Ȯ�� ġƮŰ
	if (INPUT->GetButtonDown(KEY_TYPE::I))
	{
		cout << "SpeedUp: " << _curPlayerItem[ITEM::SPEED_UP] << endl;
		cout << "Teleport: " << _curPlayerItem[ITEM::TELEPORT] << endl;
		cout << "Shield: " << _curPlayerItem[ITEM::SHIELD] << endl;
		cout << "SpeedDown: " << _curPlayerItem[ITEM::SPEED_DOWN] << endl;
		cout << "Blind: " << _curPlayerItem[ITEM::BLIND] << endl;
		cout << "Silence: " << _curPlayerItem[ITEM::SILENCE] << endl;
		cout << "Stun: " << _curPlayerItem[ITEM::STUN] << endl;
		cout << endl;
	}

	// ����� ���� ġƮŰ (���� ����)
	if (INPUT->GetButtonDown(KEY_TYPE::O))
	{
		_curPlayerItem[ITEM::SPEED_DOWN] = false;
		_curPlayerItem[ITEM::BLIND] = false;
		_curPlayerItem[ITEM::SILENCE] = false;

		_speed = _originalSpeed;
		_fBlindTime = 0.f;
		_fSilenceTime = 0.f;
		GET_SINGLE(ItemSlotManager)->IsSilenced(false);

		shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
		auto& lights = scene->GetLights();
		for (auto& light : lights)
		{
			static_pointer_cast<Light>(light)->SetDiffuse(Vec3(1.f, 1.f, 1.f));
			static_pointer_cast<Light>(light)->SetAmbient(Vec3(0.3f, 0.3f, 0.3f));
			static_pointer_cast<Light>(light)->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		}


		_state->End(*GetGameObject());
		delete _state;
		_state = new IdleState;
		_state->Enter(*GetGameObject());

		//_curPlayerItem[ITEM::STUN] = false;
	}
}

void Player::Item_SpeedUp()
{
	// ���ǵ� ���� �� �ѹ��� �ϵ���
	if (_speed != _dashSpeed)
	{
		_speed = _dashSpeed;

		_state->End(*_player);
		delete _state;
		_state = new DashState;
		_state->Enter(*_player);

		StatePkt.set_playerid(mysession->GetPlayerID());
		StatePkt.set_state(Protocol::DASH);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(StatePkt);
		mysession->Send(sendBuffer);
	}
}

void Player::Item_Teleport()
{
	// ī�޶�� �÷��̾� ���� ������ ������ �����̵�
	Vec3 pos = GetTransform()->GetLocalPosition();
	pos += GetTransform()->GetLook() * _fTeleportDis;
	GetTransform()->SetLocalPosition(pos);

	_curPlayerItem[Player::ITEM::TELEPORT] = false;
}

void Player::Item_Shield()
{
	// ���� ���¶�� ����� ���, ���� ���¶� ����� 1ȸ ���������� ���� ����

	_fShieldTime += DELTA_TIME;

	// Shield Item
	if (_fShieldTime <= 5.f)
	{
		// Shield Effect
		ShieldEffect();
	}

	else if (_fShieldTime > 5.f)
	{
		_curPlayerItem[Player::ITEM::SHIELD] = false;

		//if (mysession->GetPlayerID() == _shieldPlayerIdx)
		//	GET_SINGLE(ItemSlotManager)->UseShieldItem();
		GET_SINGLE(ShieldParticleManager)->SetShieldParticleOff();
		_fShieldTime = 0.f;
		cout << "5�� ������ ���� ��" << endl;
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

void Player::Item_Stun()
{
	cout << mysession->GetPlayerID() << " : ���� ����\n";
	/////////////// prod by. wc ///////////////
	Protocol::C_USE_STUN pkt;
	pkt.set_fromplayerid(mysession->GetPlayerID());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	mysession->Send(sendBuffer);
	///////////////////////////////////////////
}

void Player::Item_Silence()
{
	// ���� �κ�
	Protocol::C_USE_SILENCE pkt;
	pkt.set_fromplayerid(mysession->GetPlayerID());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	mysession->Send(sendBuffer);
}

void Player::SlowDown()
{
	if (CheckShield())
	{
		_curPlayerItem[Player::ITEM::SPEED_DOWN] = false;
		cout << "���� ���: SPEED_DOWN" << endl;
		GET_SINGLE(ItemSlotManager)->UseShieldItem();
		return;
	}

	// �ڽ� ���� ��� �÷��̾� 5�ʵ��� �ӵ� ����
	if (_speed != _slowSpeed)
	{
		_state->End(*GetGameObject());
		delete _state;
		_state = new SlowState;
		_state->Enter(*GetGameObject());
	}
}

void Player::Blinded()
{
	if (CheckShield())
	{
		_curPlayerItem[Player::ITEM::BLIND] = false;
		cout << "���� ���: BLIND" << endl;
		GET_SINGLE(ItemSlotManager)->UseShieldItem();
		return;
	}

	_fBlindTime += DELTA_TIME;

	// ����, �� ���� ������ �����ͼ� ��Ӱ�
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
		{
			static_pointer_cast<Light>(light)->SetDiffuse(Vec3(1.f, 1.f, 1.f));
			static_pointer_cast<Light>(light)->SetAmbient(Vec3(0.3f, 0.3f, 0.3f));
			static_pointer_cast<Light>(light)->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		}

		_fBlindTime = 0.f;
		_curPlayerItem[Player::ITEM::BLIND] = false;
	}
}

void Player::Stunned()
{
	if (CheckShield())
	{
		_curPlayerItem[Player::ITEM::STUN] = false;

		cout << "���� ���: STUN" << endl;
		GET_SINGLE(ItemSlotManager)->UseShieldItem();

		return;
	}

	// ����ũ ������ - 3�ʰ� ���� & �ؽ�ó ����
	if (!_bStunned && !_curPlayerItem[ITEM::SHIELD])
	{
		if (!isFirstEnter) {
			_state->End(*_player);
			delete _state;
			_state = new StunState;
			_state->Enter(*_player);
		}

		_bStunned = true;
	}
}

void Player::Silence() 
{
	if (CheckShield())
	{
		cout << "���� ���: SILENCE" << endl;
		GET_SINGLE(ItemSlotManager)->UseShieldItem();
		return;
	}

	_fSilenceTime += DELTA_TIME;

	// UI ����
	GET_SINGLE(ItemSlotManager)->IsSilenced(true);

	if (_fSilenceTime >= 5.f)
	{
		_fSilenceTime = 0.f;
		_curPlayerItem[Player::ITEM::SILENCE] = false;

		// UI ����
		GET_SINGLE(ItemSlotManager)->IsSilenced(false);
	}
}

void Player::ShieldEffect()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	_Shield_Effect_player = scene->GetPlayer(_shieldPlayerIdx);
	GET_SINGLE(ShieldParticleManager)->SetShieldParticleOn();

	GET_SINGLE(ShieldParticleManager)->UpdatePlayerPos(_Shield_Effect_player->GetTransform()->GetLocalPosition());
}
