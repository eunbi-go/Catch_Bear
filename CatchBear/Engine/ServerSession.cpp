#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
//#include "Player.h"

PacketSessionRef mysession;
int unsigned gPacketControl = 0;
static bool isConnected = false;
std::mutex m;

int g_EnterPlayerCnt = 0;

void ServerSession::OnConnected()
{
	Protocol::C_LOGIN pkt;
	string sNickName;
	m.lock();
	// 서버와 연결되면 플레이어 닉네임 정해줌
	if (!isConnected) {
		// 닉네임 정해주는건 후에 로비씬 생기면!
		//cout << "플레이어 닉네임을 입력하세요: ";
		//cin >> sNickName;
		//pkt.set_nickname(sNickName);
		//GPlayer.name = sNickName;

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
		isConnected = true;
	}
	m.unlock();

	//m.lock();
	//Protocol::C_ENTER_GAME pkt;
	//pkt.set_playerid(0);
	//auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	//Send(sendBuffer);
	//isConnected = true;
	//m.unlock();
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	mysession = GetPacketSessionRef();

	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);


	// TODO : packetId 대역 체크
	ServerPacketHandler::HandlePacket(mysession, buffer, len);
}

void ServerSession::OnSend(int32 len)
{
	//cout << "OnSend Len = " << len << endl;
}

void ServerSession::OnDisconnected()
{
	//cout << "Disconnected" << endl;
}
