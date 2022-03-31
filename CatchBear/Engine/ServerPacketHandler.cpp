#include "pch.h"
#include "ServerPacketHandler.h"
#include "SceneManager.h"
#include "Input.h"
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"
#include "Transform.h"
#include "Timer.h"
//#include "Session.h"
#include "ServerSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

static uint64 enterPlayerIndex = 0;
bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
		return true;

	//SceneManager::GetInstance()->MakePlayer(pkt.playerid());

	// 세션의 playerID를 저장해준다
	mysession->SetPlayerID(pkt.playerid());
	uint64 pid = mysession->GetPlayerID();

	shared_ptr<GameObject>	_player = make_shared<GameObject>();

	// 현재 씬에서 플레이어를 찾는다
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();
	_player->SetPlayerID(pkt.playerid());

	//for (auto& gameObject : gameObjects)
	//{
	//	if (gameObject->GetName() == L"Player" && gameObject->GetPlayerID() == pkt.playerid())
	//	{
	//		_player = gameObject;
	//		break;
	//	}
	//}

	//uint64 enterPlayernum = pkt.enterplayer();

	//if (enterPlayerIndex != enterPlayernum)
	//{
	//	if (enterPlayernum == 1)	// 2명 접속
	//	{
	//		if (_player->GetPlayerID() == 0)
	//			SceneManager::GetInstance()->MakePlayer(1);		// 0번 플레이어일때 1번 플레이어 만들기
	//		if (_player->GetPlayerID() == 1)
	//			SceneManager::GetInstance()->MakePlayer(0);		// 1번 플레이어일때 0번 플레이어 만들기
	//	}
	//	if (enterPlayernum == 2)	// 3명 접속
	//	{
	//		if (_player->GetPlayerID() == 0)
	//			SceneManager::GetInstance()->MakePlayer(2);		// 0번 플레이어일때 2번 플레이어 만들기
	//		if (_player->GetPlayerID() == 1)
	//			SceneManager::GetInstance()->MakePlayer(2);		// 1번 플레이어일때 2번 플레이어 만들기
	//		if (_player->GetPlayerID() == 2) {
	//			SceneManager::GetInstance()->MakePlayer(0);		// 2번 플레이어일때 0,1번 플레이어 만들기
	//			SceneManager::GetInstance()->MakePlayer(1);
	//		}
	//	}
	//	enterPlayerIndex = enterPlayernum;
	//}

	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playerid(mysession->GetPlayerID());
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

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (pkt.success() != true)
		return false;
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

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	_player = scene->GetPlayer(pkt.playerid());

	Vec3 pos;
	pos.x = pkt.xpos();
	pos.y = pkt.ypos();
	pos.z = pkt.zpos();

	//MovePkt.set_playerid(pkt.playerid());

	_player->GetTransform()->SetLocalPosition(pos);

	return true;
}
