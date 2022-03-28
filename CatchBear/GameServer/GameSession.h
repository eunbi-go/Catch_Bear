#pragma once
#include "Session.h"

//enum MoveDir {UP, DOWN, LEFT, RIGHT, END};

class GameSession : public PacketSession
{
public:
	~GameSession()
	{
		cout << "~GameSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

public:
	bool GetIsCollid() { return _isCollid; }
	void SetIsCollid(bool _Collid) { _isCollid = _Collid; }
	MoveDir GetMoveDir() { return _moveDir; }
	void SetMoveDir(MoveDir _dir) { _moveDir = _dir; }

public:
	//Vector<PlayerRef>	_players;
	PlayerRef			_player = nullptr;
	Vector<string>		_nickNames;
	bool				_isCollid = false;
	MoveDir				_moveDir;
};