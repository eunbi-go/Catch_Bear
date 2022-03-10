#pragma once
#include "Component.h"

// 우리가 직접적으로 새로 만든 커스텀 부품들 ..
class GameObject;

class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	// MonoBehaviour을 상속받은 애들이 FinalUpdate를 사용하지 못하도록 막아둠
	// 엔진에서 마지막 계산을 하고싶을 때만 사용하는 함수
	virtual void FinalUpdate() sealed {}

//public:
//	shared_ptr<GameObject> GetPlayer() { if (_player != nullptr) return _player; }
//	void SetPlayer(shared_ptr<GameObject> player) { _player = player; }
//
//private:
//	shared_ptr<GameObject>	_player;
};

