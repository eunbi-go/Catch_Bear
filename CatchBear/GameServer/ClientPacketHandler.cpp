#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "GameSession.h"
#include "Timer.h"
#include "InGame.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];
std::mutex m;

// ���� ������ �۾���
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
//	// TODO : Validation üũ
//	
	Protocol::S_LOGIN loginPkt;
	// ������ζ�� �����ͺ��̽��� �����Ͽ� ��ȿ�� ���̵����� Ȯ���ϰ� ���� ��Ŷ�� �����߰���?
	loginPkt.set_success(true);

	static Atomic<uint64> idGenerator = 0;
	{
		// PlayerId�� ���� ������
		loginPkt.set_playerid(idGenerator);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->ownerSession = gameSession;
		GInGame.Enter(playerRef);
		cout << "�÷��̾�ID " << playerRef->playerId << " �ΰ��� ���ӿϷ�!" << endl;

		gameSession->_player = playerRef;
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

#pragma region �ϴܳ��߿�
//	// gameSession�� nickName ���Ϳ� ����.
//	gameSession->_nickNames.push_back(pkt.nickname());
//	cout << "�г���: " << pkt.nickname() << " ���Ӽ���!" << endl;
//
//	// GameSession�� �÷��� ������ ���� (�޸�)
//	// ID �߱� (DB ���̵� �ƴϰ�, �ΰ��� ���̵�)
//	static Atomic<uint64> idGenerator = 0;
//	{
//		// PlayerId�� ���� ������
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
	//	cout << "�÷��̾�ID " << pkt.playerid() << " �κ� ���ӿϷ�!" << endl;

	//	enterLobbyPkt.set_isallplayersready(false);

	//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//	session->Send(sendBuffer);
	//}
	//else
	//{
	//	if (pkt.isplayerready())
	//		GLobby.SetPlayerReady(pkt.playerid());

	//	// ���� ��� �÷��̾ �غ�ƴٸ�
	//	if (GLobby.isAllPlayerReady())					
	//		enterLobbyPkt.set_isallplayersready(true);
	//	// �Ѹ��� �÷��̾�� ���������ʾ�����
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
	// ���߿��� �÷��̾�� ���ʼ��� ���ؼ� �ű⼭ Send�ϴ°ɷ� �����ؾ��ҵ�
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

// �浹�� ���� AABB�˻�
bool CheckAABB(float AX, float AZ, float BX, float BZ, float AWidth, float BWidth, float ADepth, float BDepth);

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);


	// �Ŀ��� static������Ʈ�� ��ġ �ҷ��ͼ� �÷��̾�� �浹üũ��
	// �ϴ��� �׽�Ʈ������ object��ġ
	float staticObjX = 0.f;
	float staticObjZ = 700.f;
	float staticObjWidth = 100.f;
	float staticObjDepth = 100.f;

	if (pkt.movedir() == 0)
		cout << "�÷��̾� " << pkt.playerid() << " ������ �̵�" << endl;
	if (pkt.movedir() == 1)
		cout << "�÷��̾� " << pkt.playerid() << " ������ �̵�" << endl;
	if (pkt.movedir() == 2)
		cout << "�÷��̾� " << pkt.playerid() << " �������� �̵�" << endl;
	if (pkt.movedir() == 3)
		cout << "�÷��̾� " << pkt.playerid() << " ���������� �̵�" << endl;

	cout << "x: " << pkt.xpos() << " / z: " << pkt.zpos() << endl;

	Protocol::S_MOVE movePkt;

	// ���⼭ �浹üũ�ϰ� ������ �̵� �����ϸ� �̵����ɿ��θ� ������
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

	// �浹�ϴ¼��� ������ �����ؼ� �� ������ MOVE��Ŷ ������ ����
	if (gameSession->GetIsCollid() == true) {
		movePkt.set_success(false);
		cout << "�浹��!" << endl;

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