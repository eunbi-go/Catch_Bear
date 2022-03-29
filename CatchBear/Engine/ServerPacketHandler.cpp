#include "pch.h"
#include "ServerPacketHandler.h"
#include "SceneManager.h"
#include "Input.h"
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return true;

	SceneManager::GetInstance()->MakePlayer(pkt.playerid());

	shared_ptr<GameObject>	_player = make_shared<GameObject>();

	// 현재 씬에서 플레이어를 찾는다
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player")
		{
			_player = gameObject;
			break;
		}
	}

	_player->SetPlayerID(pkt.playerid());
	cout << "이 클라이언트의 ID는 " << _player->GetPlayerID() << "입니다!" << endl;

	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerid(_player->GetPlayerID());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	//// 입장 UI 버튼 누르면 로비창 입장
	//Protocol::C_ENTER_LOBBY enterLobbyPkt;
	//enterLobbyPkt.set_playerid(GPlayer.playerId);
	//auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//session->Send(sendBuffer);

	return true;
}

bool Handle_S_ENTER_LOBBY(PacketSessionRef& session, Protocol::S_ENTER_LOBBY& pkt)
{
#pragma region test
	//// 만약 모든 플레이어가 준비됐다면 C_ENTER_GAME 패킷 보냄
	//if (pkt.isallplayersready())
	//{
	//	Protocol::C_ENTER_GAME enterGamePkt;
	//	enterGamePkt.set_playerid(GPlayer.playerId);
	//	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	//	session->Send(sendBuffer);
	//}
	//// 준비 안됐다면 여기로
	//else
	//{
	//	string sChat;
	//	// 채팅하고싶으면 컨트롤 키 누르기 (일단 임시)
	//	cout << "채팅하고싶으면 LControl키 누르세요 / 레디하고 싶으면 스페이스를 누르세요 " << endl;
	//	if (CKeyManager::Get_Instance()->Key_Down(VK_LCONTROL))
	//	{
	//		cin >> sChat;
	//		Protocol::C_CHAT chatPkt;
	//		chatPkt.set_msg(sChat);
	//		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);
	//		session->Send(sendBuffer);
	//	}
	//	if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE))
	//	{
	//		Protocol::C_ENTER_LOBBY enterLobbyPkt;
	//		enterLobbyPkt.set_playerid(GPlayer.playerId);
	//		enterLobbyPkt.set_isplayerready(true);

	//		cout << GPlayer.playerId << "번 플레이어 준비 완료!" << endl;
	//		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//		session->Send(sendBuffer);
	//	}
	//}
#pragma endregion test
	//// 만약 모든 플레이어가 준비됐다면 C_ENTER_GAME 패킷 보냄
	//if (pkt.isallplayersready())
	//{
	//	Protocol::C_ENTER_GAME enterGamePkt;
	//	enterGamePkt.set_playerid(GPlayer.playerId);
	//	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	//	session->Send(sendBuffer);
	//}
	//// 준비 안됐다면 여기로
	//else
	//{
	//	Protocol::C_ENTER_LOBBY LobbyPkt;
	//	
	//	if (!GPlayer.bisPlayerReady)
	//	{
	//		// 일단은 ready를 입력하면 준비됐다구..실제 게임에선 ui를 클릭해서 ready되도록 수정할거임.
	//		string chat;
	//		cin >> chat;
	//		if (chat == "ready")
	//		{
	//			LobbyPkt.set_playerid(GPlayer.playerId);
	//			LobbyPkt.set_isplayerready(true);
	//			GPlayer.bisPlayerReady = true;
	//			cout << "ID " << GPlayer.playerId << "준비완료" << endl;
	//		}

	//		// ready 한게 아니라면 채팅으로 간주함 실제 게임에선 채팅창 UI를 클릭하면 그때 cin을 받아 채팅 내용을 입력받음
	//		else
	//		{
	//			Protocol::C_CHAT chatPkt;
	//			chatPkt.set_playerid(GPlayer.playerId);
	//			chatPkt.set_msg(chat);
	//			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);
	//			session->Send(sendBuffer);
	//		}
	//	}
	//	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(LobbyPkt);
	//	session->Send(sendBuffer);
	//}
	return true;
}

static uint64 enterPlayerIndex = 0;
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (pkt.success() != true)
		return false;

	// TODO
	Protocol::C_ENTER_GAME enterGamePkt;
	
	shared_ptr<GameObject>	_player = make_shared<GameObject>();
	// 현재 씬에서 플레이어를 찾는다
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetName() == L"Player")
		{
			_player = gameObject;
			break;
		}
	}

	uint64 enterPlayernum = pkt.enterplayer();

	if (enterPlayerIndex != enterPlayernum)
	{
		if (enterPlayernum == 1)	// 2명 접속
		{
			if (_player->GetPlayerID() == 0)
				SceneManager::GetInstance()->MakePlayer(1);		// 0번 플레이어일때 1번 플레이어 만들기
			if (_player->GetPlayerID() == 1)
				SceneManager::GetInstance()->MakePlayer(0);		// 1번 플레이어일때 0번 플레이어 만들기
		}
		if (enterPlayernum == 2)	// 3명 접속
		{
			if (_player->GetPlayerID() == 0)
				SceneManager::GetInstance()->MakePlayer(2);		// 0번 플레이어일때 2번 플레이어 만들기
			if (_player->GetPlayerID() == 1)
				SceneManager::GetInstance()->MakePlayer(2);		// 1번 플레이어일때 2번 플레이어 만들기
			if (_player->GetPlayerID() == 2) {
				SceneManager::GetInstance()->MakePlayer(0);		// 2번 플레이어일때 0,1번 플레이어 만들기
				SceneManager::GetInstance()->MakePlayer(1);
			}
		}
		enterPlayerIndex = enterPlayernum;
	}

	//_player->SetPlayerID(pkt.playerid());
	enterGamePkt.set_playerid(_player->GetPlayerID());
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	//uint64 playerId = pkt.playerid();

	//std::cout << "ID: " << playerId << ") " << pkt.msg() << endl;

	////std::cout << pkt.msg() << endl;
	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	shared_ptr<GameObject>	_player = make_shared<GameObject>();
	Protocol::C_MOVE MovePkt;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	//const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
	_player = scene->GetPlayer(pkt.playerid());

	// 만약 충돌해서 위치가 바뀐다면 위치 다시 바꿔준다.

	//Vec3 pos = _player->GetTransform()->GetLocalPosition();
	Vec3 pos;
	pos.x = pkt.xpos();
	pos.z = pkt.zpos();

	// 여기서 서버에서 받은 위치로 player위치를 바꿔주는 코드가 들어감
	// 만약 충돌한 상태라면 success가 false로 오겠지?
	if (pkt.success() == true)
	{
		if (pkt.movedir() == 0)
		{
			//pos += _player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
			//_player->GetTransform()->SetLocalPosition(pos);
		}
		if (pkt.movedir() == 1)
		{
			//pos -= _player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
			//_player->GetTransform()->SetLocalPosition(pos);
		}
	}
	else
	{
		// 앞으로 충돌, 뒤,왼,오 이동가능
		if (pkt.movedir() == 0)
		{
			if (INPUT->GetButton(KEY_TYPE::DOWN))
			{
				//pos -= _player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
				//_player->GetTransform()->SetLocalPosition(pos);
			}
		}
		// 뒤로 충돌, 앞,왼,오 이동가능
		if (pkt.movedir() == 1)
		{
			if (INPUT->GetButton(KEY_TYPE::UP))
			{
				//pos += _player->GetTransform()->GetLook() * 5.f * DELTA_TIME;
				//_player->GetTransform()->SetLocalPosition(pos);
			}
		}
	}

	_player->GetTransform()->SetLocalPosition(pos);

	return true;
}
