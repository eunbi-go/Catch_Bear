#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Lobby.h"
#include "GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

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

	// TODO : Validation 체크

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// gameSession의 nickName 벡터에 저장.
	gameSession->_nickNames.push_back(pkt.nickname());
	cout << "닉네임: " << pkt.nickname() << " 접속성공!" << endl;

	// GameSession에 플레이 정보를 저장 (메모리)
	// ID 발급 (DB 아이디가 아니고, 인게임 아이디)
	static Atomic<uint64> idGenerator = 0;
	{
		// PlayerId도 같이 보내줌
		loginPkt.set_playerid(idGenerator);

		auto player = loginPkt.add_players();
		player->set_nickname(pkt.nickname());
		player->set_playertype(Protocol::PLAYER_TYPE_BEAR1);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->nickname = player->nickname();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}
#pragma region addPlayer
	/*{
		auto player = loginPkt.add_players();
		player->set_name(u8"DB에서긁어온이름1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;
		
		gameSession->_players.push_back(playerRef);
	}

	{
		auto player = loginPkt.add_players();
		player->set_name(u8"DB에서긁어온이름2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}*/
#pragma endregion addPlayer

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_LOBBY(PacketSessionRef& session, Protocol::C_ENTER_LOBBY& pkt)
{
	//GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	//cout << "플레이어ID " << pkt.playerid() << " 로비 접속완료!" << endl;

	//uint64 index = pkt.playerid();
	//PlayerRef player = gameSession->_players[index]; // READ_ONLY?
	//GLobby.Enter(player); // WRITE_LOCK

	//Protocol::S_ENTER_LOBBY enterLobbyPkt;

	//// 만약 모든 플레이어가 준비됐다면
	////enterLobbyPkt.set_isallplayersready(true);
	//// 한명의 플레이어라도 레디하지않았으면
	//enterLobbyPkt.set_isallplayersready(false);

	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterLobbyPkt);
	//session->Send(sendBuffer);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	//GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	//uint64 index = pkt.playerindex();
	//// TODO : Validation

	//PlayerRef player = gameSession->_players[index]; // READ_ONLY?
	//GLobby.Enter(player); // WRITE_LOCK

	//Protocol::S_ENTER_GAME enterGamePkt;
	//enterGamePkt.set_success(true);
	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	//player->ownerSession->Send(sendBuffer);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GLobby.Broadcast(sendBuffer); // WRITE_LOCK

	return true;
}
