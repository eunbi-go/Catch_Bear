#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "GameSession.h"
#include "Timer.h"
#include "InGame.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];
std::mutex m;

// 직접 컨텐츠 작업자
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
//
//	// TODO : Validation 체크
//	
	Protocol::S_LOGIN loginPkt;
	// 원래대로라면 데이터베이스에 접근하여 유효한 아이디인지 확인하고 성공 패킷을 보내야겠죠?
	loginPkt.set_success(true);

	static Atomic<uint64> idGenerator = 0;
	{
		// PlayerId도 같이 보내줌
		loginPkt.set_playerid(idGenerator);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->ownerSession = gameSession;
		GInGame.Enter(playerRef);
		cout << "플레이어ID " << playerRef->playerId << " 인게임 접속완료!" << endl;

		gameSession->_player = playerRef;
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

#pragma region 일단나중에
//	// gameSession의 nickName 벡터에 저장.
//	gameSession->_nickNames.push_back(pkt.nickname());
//	cout << "닉네임: " << pkt.nickname() << " 접속성공!" << endl;
//
//	// GameSession에 플레이 정보를 저장 (메모리)
//	// ID 발급 (DB 아이디가 아니고, 인게임 아이디)
//	static Atomic<uint64> idGenerator = 0;
//	{
//		// PlayerId도 같이 보내줌
//		loginPkt.set_playerid(idGenerator);
//
//		auto player = loginPkt.add_players();
//		player->set_nickname(pkt.nickname());
//		player->set_playertype(Protocol::PLAYER_TYPE_BEAR1);
//
//		PlayerRef playerRef = MakeShared<Player>();
//		playerRef->playerId = idGenerator++;
//		playerRef->nickname = player->nickname();
//		playerRef->type = player->playertype();
//		playerRef->ownerSession = gameSession;
//
//		gameSession->_player = playerRef;
//	}

//#pragma endregion addPlayer
//
//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
//	session->Send(sendBuffer);
#pragma endregion
	return true;
}

bool Handle_C_ENTER_LOBBY(PacketSessionRef& session, Protocol::C_ENTER_LOBBY& pkt)
{
	//GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
	//Protocol::S_ENTER_LOBBY enterLobbyPkt;
	//if (GLobby.isFirstEnterLobby(pkt.playerid()))
	//{
	//	PlayerRef player = gameSession->_player;
	//	player->playerId = pkt.playerid();
	//	GLobby.Enter(player);
	//	cout << "플레이어ID " << pkt.playerid() << " 로비 접속완료!" << endl;

	//	enterLobbyPkt.set_isallplayersready(false);

	//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//	session->Send(sendBuffer);
	//}
	//else
	//{
	//	if (pkt.isplayerready())
	//		GLobby.SetPlayerReady(pkt.playerid());

	//	// 만약 모든 플레이어가 준비됐다면
	//	if (GLobby.isAllPlayerReady())					
	//		enterLobbyPkt.set_isallplayersready(true);
	//	// 한명의 플레이어라도 레디하지않았으면
	//	else
	//		enterLobbyPkt.set_isallplayersready(false);

	//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//	session->Send(sendBuffer);
	//}
	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);

	enterGamePkt.set_enterplayer(GInGame.GetEnterPlayerNum() - 1);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	// 나중에는 플레이어마다 오너세션 정해서 거기서 Send하는걸로 수정해야할듯
	//player->ownerSession->Send(sendBuffer);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	//uint64 playerId = pkt.playerid();

	//std::cout << "ID: " << playerId << ") " << pkt.msg() << endl;

	//Protocol::S_CHAT chatPkt;
	//chatPkt.set_playerid(playerId);
	//chatPkt.set_msg(pkt.msg());
	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	//GLobby.Broadcast(sendBuffer); // WRITE_LOCK

	return true;
}

// 충돌을 위한 AABB검사
bool CheckAABB(float AX, float AZ, float BX, float BZ, float AWidth, float BWidth, float ADepth, float BDepth);

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);


	// 후에는 static오브젝트들 위치 불러와서 플레이어랑 충돌체크함
	// 일단은 테스트용으로 object위치
	float staticObjX = 0.f;
	float staticObjZ = 700.f;
	float staticObjWidth = 100.f;
	float staticObjDepth = 100.f;

	if (pkt.movedir() == 0)
		cout << "플레이어 " << pkt.playerid() << " 앞으로 이동" << endl;
	if (pkt.movedir() == 1)
		cout << "플레이어 " << pkt.playerid() << " 뒤으로 이동" << endl;
	if (pkt.movedir() == 2)
		cout << "플레이어 " << pkt.playerid() << " 왼쪽으로 이동" << endl;
	if (pkt.movedir() == 3)
		cout << "플레이어 " << pkt.playerid() << " 오른쪽으로 이동" << endl;

	cout << "x: " << pkt.xpos() << " / z: " << pkt.zpos() << endl;

	Protocol::S_MOVE movePkt;

	// 여기서 충돌체크하고 앞으로 이동 가능하면 이동가능여부만 보내줌
	if (gameSession->GetIsCollid() == false)
	{
		bool isCollid = CheckAABB(pkt.xpos(), pkt.zpos(), staticObjX, staticObjZ, 50.f, staticObjWidth, 50.f, staticObjDepth);
		gameSession->SetIsCollid(isCollid);
		movePkt.set_success(true);
		switch (pkt.movedir())
		{
		case 0:
			gameSession->SetMoveDir(MoveDir::UP);
			break;
		case 1:
			gameSession->SetMoveDir(MoveDir::DOWN);
			break;
		case 2:
			gameSession->SetMoveDir(MoveDir::LEFT);
			break;
		case 3:
			gameSession->SetMoveDir(MoveDir::RIGHT);
			break;
		}
		movePkt.set_movedir(pkt.movedir());
	}

	// 충돌하는순간 방향을 저장해서 그 방향은 MOVE패킷 보내지 않음
	if (gameSession->GetIsCollid() == true) {
		movePkt.set_success(false);
		cout << "충돌함!" << endl;

		switch (gameSession->GetMoveDir())
		{
		case MoveDir::UP:
			movePkt.set_movedir(0);
			break;
		case MoveDir::DOWN:
			movePkt.set_movedir(1);
			break;
		case MoveDir::LEFT:
			movePkt.set_movedir(2);
			break;
		case MoveDir::RIGHT:
			movePkt.set_movedir(3);
			break;
		}

		bool isCollid = CheckAABB(pkt.xpos(), pkt.zpos(), staticObjX, staticObjZ, 50.f, staticObjWidth, 50.f, staticObjDepth);
		if (!isCollid)
		{
			gameSession->SetIsCollid(isCollid);
		}
	}

	movePkt.set_playerid(pkt.playerid());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
	
	GInGame.Broadcast(sendBuffer);
	
	//gameSession->Send(sendBuffer);
	return true;
}

bool CheckAABB(float AX, float AZ, float BX, float BZ, float AWidth, float BWidth, float ADepth, float BDepth)
{
	float ALeft = AX - (AWidth / 2);
	float BLeft = BX - (BWidth / 2);
	float AFront = AZ - (ADepth / 2);
	float BFront = BZ - (BDepth / 2);

	if (BLeft + BWidth > ALeft && ALeft + AWidth > BLeft
		&& BFront + BDepth > AFront && AFront + ADepth > BFront)
	{
		return true;
	}
	return false;
}